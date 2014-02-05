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

extern "C" {
#include <libavformat/avio.h>
#include <libavutil/mem.h>
#include <libavcodec/avcodec.h>
};

MediaDecoder::MediaDecoder(MediaPlaylist *list, QObject *parent) :
  QObject(parent),
  m_list(list),
  m_finish(false) {

}

MediaDecoder::~MediaDecoder() {

}

void MediaDecoder::process() {
  const QList<Media *> media = m_list->media();

  for (int x = 0; x < media.size(); x++) {
    const Media *m = media[x];
    QByteArray data = m_list->recitation()->data(m);
    if (data.isEmpty()) {
      emit error();
      return;
    }

    AVFormatContext *ctx = context(data);
    if (!ctx) {
      emit error();
      return;
    }

    if (!decode(ctx, m)) {
      cleanup(ctx);

      if (!finishRequested()) {
	emit error();
      } else {
	emit finished();
      }

      return;
    }

    cleanup(ctx);
  }

  emit finished();
}

bool MediaDecoder::decode(AVFormatContext *ctx, const Media *media) {
  AVCodecContext* codec_ctx = ctx->streams[0]->codec;
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

  AVPacket pkt;
  av_init_packet(&pkt);
  int buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE;
  uint8_t buffer[buffer_size];
  pkt.data = buffer;
  pkt.size = buffer_size;

  while (av_read_frame(ctx, &pkt) >= 0) {
    if (!decode(codec_ctx, &pkt, media)) {
      avcodec_close(codec_ctx);
      return false;
    }

    if (finishRequested()) {
      return false;
    }

    pkt.data = buffer;
    pkt.size = buffer_size;
  }

  avcodec_close(codec_ctx);

  return true;
}

bool MediaDecoder::decode(AVCodecContext *ctx, AVPacket *pkt, const Media *media) {
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
      return false;
    }

    if (got_frame) {
      int data_size = av_samples_get_buffer_size(NULL, ctx->channels,
						 frame->nb_samples,
						 ctx->sample_fmt, 1);
      AudioBuffer *b = new AudioBuffer;;
      b->index = media->index();
      b->rate = frame->sample_rate;
      b->channels = ctx->channels;
      b->data = QByteArray(reinterpret_cast<char *>(frame->data[0]), data_size);
      b->chapter = media->chapter();
      b->verse = media->verse();
      emit play(b);
    }

    pkt->size -= len;
    pkt->data += len;

    if (finishRequested()) {
      return false;
    }

    if (pkt->size == 0) {
      return true;
    }
  }

  return true;
}

void MediaDecoder::cleanup(AVFormatContext *ctx) {
  // TODO:
}

void MediaDecoder::finish() {
  m_mutex.lock();
  m_finish = true;
  m_mutex.unlock();
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

  err = avformat_open_input(&ctx, "", NULL, NULL);
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

bool MediaDecoder::finishRequested() {
  QMutexLocker locker(&m_mutex);

  return m_finish;
}
