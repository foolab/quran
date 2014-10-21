/*
 * Copyright (c) 2011-2014 Mohammed Sameer <msameer@foolab.org>.
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

#include "mediadecoder.h"
#include "media.h"
#include "mediaplaylist.h"
#include "recitation.h"
#include <QDebug>
#include <QBuffer>
#include "audiooutput.h"
#include "mediaresampler.h"

extern "C" {
#include <libavformat/avio.h>
#include <libavutil/mem.h>
#include <libavcodec/avcodec.h>
};

MediaDecoder::MediaDecoder(QObject *parent) :
  QThread(parent),
  m_audio(0),
  m_stop(false) {

}

MediaDecoder::~MediaDecoder() {

}

void MediaDecoder::run() {
  while (true) {
    Media media = MediaDecoder::media();
    if (media.isEos()) {
      // We are done.
      play(AudioBuffer(Media::eos()));
      return;
    }
    else if (media.isError()) {
      play(AudioBuffer(Media::error()));
      return;
    }

    QByteArray data = media.data();

    if (data.isEmpty()) {
      play(AudioBuffer(Media::error()));
      return;
    }

    AVFormatContext *ctx = context(data);
    if (!ctx) {
      play(AudioBuffer(Media::error()));
      return;
    }

    if (!decode(ctx, media)) {
      cleanup(ctx);
      play(AudioBuffer(Media::error()));
      return;
    }

    cleanup(ctx);
  }
}

bool MediaDecoder::decode(AVFormatContext *ctx, const Media& media) {
  AVCodecContext *codec_ctx = ctx->streams[0]->codec;
  codec_ctx->request_sample_fmt = AV_SAMPLE_FMT_S16;

  AVCodec* codec = avcodec_find_decoder(codec_ctx->codec_id);
  if (!codec) {
    return false;
  }

  if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
    return false;
  }

  if (codec_ctx->request_sample_fmt != AV_SAMPLE_FMT_S16) {
    avcodec_close(codec_ctx);
    return false;
  }

  MediaResampler *resampler = 0;
  AVPacket pkt;
  av_init_packet(&pkt);
  int buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE;
  uint8_t buffer[buffer_size];
  pkt.data = buffer;
  pkt.size = buffer_size;

  AudioBuffer b(media);

  while (av_read_frame(ctx, &pkt) >= 0) {
    if (!decode(codec_ctx, &pkt, b, &resampler)) {
      avcodec_close(codec_ctx);
      if (resampler) {
	delete resampler;
      }

      return false;
    }

    pkt.data = buffer;
    pkt.size = buffer_size;
  }

  if (resampler) {
    delete resampler;
  }

  avcodec_close(codec_ctx);

  if (b.data.isEmpty()) {
    // I can only think of one valid scenario which is every single input packed has been invalid!
    // Let's just fail here :/
    return false;
  }

  play(b);

  return true;
}

bool MediaDecoder::decode(AVCodecContext *ctx, AVPacket *pkt,
			  AudioBuffer& buffer, MediaResampler **resampler) {
  AVFrame *frame = avcodec_alloc_frame();
  if (!frame) {
    return false;
  }

  avcodec_get_frame_defaults(frame);

  int len = 0;
  int got_frame = 0;

  while (len >= 0) {
    len = avcodec_decode_audio4(ctx, frame, &got_frame, pkt);
    if (len < 0) {
      avcodec_free_frame(&frame);
      // We will skip the packet but just return true so we don't signal an error
      return true;
    }

    if (got_frame) {
      int data_size = av_samples_get_buffer_size(NULL, ctx->channels,
						 frame->nb_samples,
						 ctx->sample_fmt, 1);
      Q_UNUSED(data_size);

      QByteArray data;
      if (!(*resampler)) {
	*resampler = MediaResampler::create(ctx);
	if (!(*resampler)) {
	  qWarning() << "Failed to create resampler";
	  avcodec_free_frame(&frame);
	  return false;
	}
      }

      if (!(*resampler)->resample(frame, data)) {
	avcodec_free_frame(&frame);
	return false;
      }

      buffer.data.append(data);
    }

    pkt->size -= len;
    pkt->data += len;

    if (pkt->size == 0) {
      avcodec_free_frame(&frame);
      return true;
    }
  }

  avcodec_free_frame(&frame);
  return true;
}

void MediaDecoder::cleanup(AVFormatContext *ctx) {
  AVIOContext *io = ctx->pb;
  unsigned char *buffer = io->buffer;
  QBuffer *b = reinterpret_cast<QBuffer *>(io->opaque);

  if (b) {
    delete b;
  }

  if (buffer) {
    av_free(buffer);
  }

  av_free(io);

  av_free(ctx);
}

void MediaDecoder::stop() {
  QMutexLocker locker(&m_mutex);
  m_stop = true;
  m_cond.wakeOne();
}

int read_qbuffer(void *opaque, uint8_t *buf, int buf_size) {
  QBuffer *b = reinterpret_cast<QBuffer *>(opaque);

  int ret = b->read((char *)buf, buf_size);

  return ret == 0 ? AVERROR_EOF : ret;
}

int64_t seek_qbuffer(void *opaque, int64_t offset, int whence) {
  QBuffer *b = reinterpret_cast<QBuffer *>(opaque);

  switch (whence) {
  case SEEK_SET:
    return b->seek(offset) ? b->pos() : -1;

  case SEEK_CUR:
    return b->seek(b->pos() + offset) ? b->pos() : -1;

  case SEEK_END:
    return -1;

  case AVSEEK_SIZE:
    return b->size();

  default:
    return -1;
  }
}

AVFormatContext *MediaDecoder::context(const QByteArray& data) {
  QBuffer *b = 0;
  unsigned char *buff = 0;
  AVIOContext *io = 0;
  AVFormatContext *ctx = 0;
  int err = 0;

  b = new QBuffer;
  b->setData(data);
  b->open(QBuffer::ReadOnly);

  buff = (unsigned char *)av_malloc(4096);
  io = avio_alloc_context(buff, 4096, 0, b, read_qbuffer, NULL, seek_qbuffer);

  ctx = avformat_alloc_context();
  ctx->pb = io;
  ctx->probesize = 4096;
  ctx->flags |= AVFMT_FLAG_CUSTOM_IO;

  err = avformat_open_input(&ctx, "foo.mp3", NULL, NULL);
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

  if (b) {
    delete b;
  }

  if (buff) {
    av_free(buff);
  }

  if (io) {
    av_free(io);
  }

  if (ctx) {
    av_free(ctx);
  }

  return 0;
}

void MediaDecoder::addMedia(const Media& media) {
  QMutexLocker locker(&m_mutex);

  m_media << media;
  m_cond.wakeOne();
}

Media MediaDecoder::media() {
  QMutexLocker locker(&m_mutex);

  if (m_stop) {
    return Media::eos();
  }

  if (m_media.isEmpty()) {
    m_cond.wait(&m_mutex);
  }

  if (m_stop) {
    return Media::eos();
  }

  if (!m_media.isEmpty()) {
    return m_media.takeFirst();
  }

  return Media::eos();
}

bool MediaDecoder::stopRequested() {
  QMutexLocker locker(&m_mutex);
  return m_stop;
}

void MediaDecoder::setOutput(AudioOutput *audio) {
  QMutexLocker locker(&m_audioMutex);
  m_audio = audio;

  foreach (const AudioBuffer& buffer, m_buffers) {
    m_audio->play(buffer);
  }

  m_buffers.clear();
}

void MediaDecoder::play(const AudioBuffer& buffer) {
  QMutexLocker locker(&m_audioMutex);
  if (m_audio) {
    m_audio->play(buffer);
  }
  else {
    m_buffers << buffer;
  }
}
