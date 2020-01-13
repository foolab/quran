/*
 * Copyright (c) 2019-2020 Mohammed Sameer <msameer@foolab.org>.
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
#define TIMER_INTERVAL_MS            50

#ifndef PAGE_SIZE
#define PAGE_SIZE                    4096
#endif

int read_qbuffer(void *opaque, uint8_t *buf, int buf_size);

MediaCodec::MediaCodec(Media media, QByteArray& data, QObject *parent) :
  QObject(parent),
  m_media(media),
  m_resampler(0),
  m_ctx(0),
  m_codec(0) {

  m_ctx = context(data);
  if (!m_ctx) {
    m_buffers << AudioBuffer(Media::error());
    return;
  }

  int stream_index = av_find_best_stream(m_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
  if (stream_index < 0) {
    qWarning() << "Cannot find audio stream to decode";
    m_buffers << AudioBuffer(Media::error());
    return;
  }

  AVStream *stream = m_ctx->streams[stream_index];

  AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
  if (!codec) {
    qWarning() << "Cannot find audio decoder for stream";
    m_buffers << AudioBuffer(Media::error());
    return;
  }

  m_codec = avcodec_alloc_context3(codec);
  if (!m_codec) {
    qWarning() << "Cannot allocate audio decoder for stream";
    m_buffers << AudioBuffer(Media::error());
    return;
  }

  if (avcodec_parameters_to_context(m_codec, stream->codecpar) < 0) {
    qWarning() << "Cannot set audio decoder parameters for stream";
    avcodec_free_context(&m_codec);
    m_codec = 0;
    m_buffers << AudioBuffer(Media::error());
    return;
  }

  m_codec->request_sample_fmt = AV_SAMPLE_FMT_S16;
  AVDictionary *opts = NULL;
  av_dict_set(&opts, "refcounted_frames", "1", 0);
  if (avcodec_open2(m_codec, codec, &opts) < 0) {
    qWarning() << "Cannot open audio decoder for stream";
    avcodec_free_context(&m_codec);
    m_codec = 0;
    m_buffers << AudioBuffer(Media::error());
    return;
  }

  if (m_codec->request_sample_fmt != AV_SAMPLE_FMT_S16) {
    qWarning() << "Cannot set decoder output sample format for stream";
    avcodec_close(m_codec);
    m_codec = 0;
    m_buffers << AudioBuffer(Media::error());
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

      if (n == AVERROR_EOF) {
	// EOF -> flush
	m_timer.stop();
	flushDecoder(b);
	goto out;
      } else {
	qWarning() << "Error decoding packet";
	// Error:
	goto error;
      }
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
    qWarning() << "Empty data decoding packet";
    goto error;
  }

out:
  m_buffers << b;
  emit buffersAvailable();
  return;

error:
  flushDecoder(b);
  m_buffers << b;
  m_buffers << AudioBuffer(Media::error());
  m_timer.stop();
  emit buffersAvailable();
}

void MediaCodec::flushDecoder(AudioBuffer& buffer) {
  AVPacket *pkt = av_packet_alloc();
  if (pkt) {
    pkt->data = NULL;
    pkt->size = 0;
    decode(pkt, buffer);
    av_packet_free(&pkt);
  }
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
    qWarning() << "Cannot allocate audio frame";
    return false;
  }

  int ret = avcodec_send_packet(m_codec, pkt);
  if (ret < 0) {
    qWarning() << "Failed to send packet to decoder";
    av_frame_free(&frame);
    return false;
  }

  while (ret >= 0) {
    ret = avcodec_receive_frame(m_codec, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
      av_frame_free(&frame);
      return true;
    }

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
      qWarning() << "Failed to resample audio frame";
      av_frame_free(&frame);
      return false;
    }

    buffer.data.append(data);
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
    avcodec_free_context(&m_codec);
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
