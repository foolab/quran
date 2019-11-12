/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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

#include "mediaresampler.h"

extern "C" {
#include <libavutil/opt.h>
#include <libavutil/mem.h>
};

MediaResampler::MediaResampler(QObject *parent) :
  QObject(parent),
  m_ctx(0),
  m_bypass(false) {
}

MediaResampler::~MediaResampler() {
  if (m_ctx) {
    swr_close(m_ctx);
    swr_free(&m_ctx);
    m_ctx = 0;
  }
}

MediaResampler *MediaResampler::create(AVCodecContext *ctx, QObject *parent) {
  MediaResampler *resampler = new MediaResampler(parent);

  if (resampler->init(ctx)) {
    return resampler;
  }

  delete resampler;
  return 0;
}

bool MediaResampler::init(AVCodecContext *ctx) {
  if (ctx->channels == 1 && ctx->sample_rate == 22050 && ctx->sample_fmt == AV_SAMPLE_FMT_S16) {
    m_bypass = true;
    return true;
  }

  m_ctx = swr_alloc();
  if (!m_ctx) {
    return false;
  }

  av_opt_set_int(m_ctx, "out_channel_layout", av_get_default_channel_layout(1), 0);
  av_opt_set_int(m_ctx, "out_sample_rate", 22050, 0);
  av_opt_set_int(m_ctx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

  av_opt_set_int(m_ctx, "in_sample_fmt", AV_SAMPLE_FMT_S16, 0);
  av_opt_set_int(m_ctx, "in_channel_layout", av_get_default_channel_layout(ctx->channels), 0);
  av_opt_set_int(m_ctx, "in_sample_rate", ctx->sample_rate, 0);

  if (swr_init(m_ctx) < 0) {
    swr_free(&m_ctx);
    m_ctx = 0;
    return false;
  }

  return true;
}

bool MediaResampler::resample(AVFrame *frame, QByteArray& output) {
  if (m_bypass) {
    int data_size = av_samples_get_buffer_size(NULL, 1,
					       frame->nb_samples,
					       AV_SAMPLE_FMT_S16, 1);

    output.append(QByteArray(reinterpret_cast<char *>(frame->data[0]), data_size));
    return true;
  }

  int out_line = 0;
  int out_size = av_samples_get_buffer_size(&out_line, 1, frame->nb_samples, AV_SAMPLE_FMT_S16, 0);
  uint8_t *out = (uint8_t *) av_malloc(out_size);
  if (!out) {
    return false;
  }

  uint8_t **out_t = &out;
  // Now we resample:
  int nb_samples = swr_convert(m_ctx, out_t, frame->nb_samples,
			       (const uint8_t **)frame->extended_data, frame->nb_samples);

  if (nb_samples < 0) {
    av_free(out);
    return false;
  }

  // Initial data
  output.append(QByteArray(reinterpret_cast<char *>(out),
			   av_get_bytes_per_sample(AV_SAMPLE_FMT_S16) * nb_samples));

  // Any data remaining (delay)
  out_t = &out;
  nb_samples = swr_convert(m_ctx, out_t, frame->nb_samples, NULL, 0);

  if (nb_samples < 0) {
    av_free(out);
    return false;
  }

  output.append(QByteArray(reinterpret_cast<char *>(out),
			   av_get_bytes_per_sample(AV_SAMPLE_FMT_S16) * nb_samples));

  av_free(out);
  return true;
}
