/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mediacodec.h"
#include <QBuffer>
#include <QDebug>
#include <QElapsedTimer>
#include "mediaresampler.h"

extern "C" {
#include <libavformat/avio.h>
#include <libavutil/mem.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

#define MAX_DECODE_TIME_MS           5
#define PAGE_SIZE                    4096
#define TIMER_INTERVAL_MS            50

int read_qbuffer(void *opaque, uint8_t *buf, int buf_size);

MediaCodec::MediaCodec(Media media, QObject *parent) :
  QObject(parent),
  m_media(media),
  m_resampler(0),
  m_ctx(0),
  m_codec(0) {

  m_ctx = context(media.data());
  if (!m_ctx) {
    m_buffers << AudioBuffer(Media::error());
    m_timer.stop();
    emit buffersAvailable();
    return;
  }

  m_codec = m_ctx->streams[0]->codec;
  m_codec->request_sample_fmt = AV_SAMPLE_FMT_S16;

  AVCodec *codec = avcodec_find_decoder(m_codec->codec_id);
  if (!codec) {
    m_buffers << AudioBuffer(Media::error());
    m_timer.stop();
    emit buffersAvailable();
    return;
  }

  if (avcodec_open2(m_codec, codec, NULL) < 0) {
    m_codec = 0;
    m_buffers << AudioBuffer(Media::error());
    m_timer.stop();
    emit buffersAvailable();
    return;
  }

  if (m_codec->request_sample_fmt != AV_SAMPLE_FMT_S16) {
    avcodec_close(m_codec);
    m_codec = 0;
    m_buffers << AudioBuffer(Media::error());
    m_timer.stop();
    emit buffersAvailable();
    return;
  }

  m_timer.setSingleShot(false);
  m_timer.setInterval(TIMER_INTERVAL_MS);
  m_timer.setTimerType(Qt::PreciseTimer);

  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(decodePacket()));

  m_timer.start();
}

MediaCodec::~MediaCodec() {
  cleanup();
}

void MediaCodec::decodePacket() {
  AVPacket pkt;
  av_init_packet(&pkt);
  pkt.data = NULL;
  pkt.size = 0;

  AudioBuffer b(m_media);

  QElapsedTimer timer;
  timer.start();
  while (!timer.hasExpired(MAX_DECODE_TIME_MS)) {
    int n = av_read_frame(m_ctx, &pkt);
    if (n < 0) {
      av_packet_unref(&pkt);
      // TODO: how do we detect an error?
      m_timer.stop();
      goto out;
    }

    if (!decode(&pkt, b)) {
      avcodec_close(m_codec);
      av_packet_unref(&pkt);
      goto error;
    }

    av_packet_unref(&pkt);
  }

  if (b.data.isEmpty()) {
    // I can only think of one valid scenario which is every single input packed has been invalid!
    // Let's just fail here :/
    goto error;
  }

out:
  m_buffers << b;
  emit buffersAvailable();

  return;

error:
  m_buffers << AudioBuffer(Media::error());
  m_timer.stop();
  emit buffersAvailable();
}

void MediaCodec::stop() {
  m_timer.stop();
}

bool MediaCodec::isDone() {
  return !m_timer.isActive();
}

QList<AudioBuffer> MediaCodec::buffers() {
  QList<AudioBuffer> buffers = m_buffers;
  m_buffers.clear();
  return buffers;
}

bool MediaCodec::decode(AVPacket *pkt, AudioBuffer& buffer) {
  AVFrame *frame = av_frame_alloc();
  if (!frame) {
    return false;
  }

  int len = 0;
  int got_frame = 0;

  while (len >= 0) {
    len = avcodec_decode_audio4(m_codec, frame, &got_frame, pkt);
    if (len < 0) {
      av_frame_free(&frame);
      // We will skip the packet but just return true so we don't signal an error
      return true;
    }

    if (got_frame) {
      int data_size = av_samples_get_buffer_size(NULL, m_codec->channels,
						 frame->nb_samples,
						 m_codec->sample_fmt, 1);
      Q_UNUSED(data_size);

      QByteArray data;
      if (!m_resampler) {
	m_resampler = MediaResampler::create(m_codec);
	if (!m_resampler) {
	  qWarning() << "Failed to create resampler";
	  av_frame_free(&frame);
	  return false;
	}
      }

      if (!m_resampler->resample(frame, data)) {
	av_frame_free(&frame);
	return false;
      }

      buffer.data.append(data);
    }

    pkt->size -= len;
    pkt->data += len;

    if (pkt->size == 0) {
      av_frame_free(&frame);
      return true;
    }
  }

  av_frame_free(&frame);
  return true;
}

AVFormatContext *MediaCodec::context(const QByteArray& data) {
  QBuffer *b = 0;
  unsigned char *buff = 0;
  AVIOContext *io = 0;
  AVFormatContext *ctx = 0;
  int err = 0;

  b = new QBuffer;
  b->setData(data);
  b->open(QBuffer::ReadOnly);

  buff = (unsigned char *)av_malloc(PAGE_SIZE);
  if (!buff) {
    goto error_and_out;
  }

  io = avio_alloc_context(buff, PAGE_SIZE, 0, b, read_qbuffer, NULL, NULL);
  if (!io) {
    goto error_and_out;
  }

  // Now that we have a AVIOContext, we will free its buffer
  // member when we free it in case of an error
  buff = NULL;

  ctx = avformat_alloc_context();
  ctx->pb = io;
  ctx->probesize = PAGE_SIZE;
  ctx->flags |= AVFMT_FLAG_CUSTOM_IO;

  err = avformat_open_input(&ctx, NULL, NULL, NULL);
  if (err != 0) {
    goto error_and_out;
  }

  err = avformat_find_stream_info(ctx, NULL);
  if (err < 0) {
    goto error_and_out;
  }

  if (ctx->nb_streams != 1) {
    qWarning() << "Can only deal with 1 stream";
    goto error_and_out;
  }

  return ctx;

 error_and_out:
  char error[1000];
  av_strerror (err, error, 999);
  qWarning() << "Error" << error;

  avformat_close_input(&ctx);
  if (b) {
    delete b;
    b = 0;
  }

  if (buff) {
    av_free(buff);
  }

  if (io) {
    if (io->buffer) {
      av_freep(&io->buffer);
    }

    avio_context_free(&io);
  }

  if (io) {
    av_free(io);
  }

  if (ctx) {
    av_free(ctx);
  }

  return 0;
}

void MediaCodec::cleanup() {
  AVIOContext *io = m_ctx->pb;
  QBuffer *b = reinterpret_cast<QBuffer *>(io->opaque);

  if (m_ctx) {
    avformat_close_input(&m_ctx);
    m_ctx = 0;
  }

  if (m_codec) {
    avcodec_close(m_codec);
    m_codec = 0;
  }

  if (b) {
    delete b;
    b = 0;
  }

  if (io) {
    if (io->buffer) {
      av_freep(&io->buffer);
    }
    avio_context_free(&io);
  }

  if (m_resampler) {
    delete m_resampler;
    m_resampler = 0;
  }
}

int read_qbuffer(void *opaque, uint8_t *buf, int buf_size) {
  QBuffer *b = reinterpret_cast<QBuffer *>(opaque);

  int ret = b->read((char *)buf, buf_size);

  return ret == 0 ? AVERROR_EOF : ret;
}
