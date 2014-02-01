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

#include "mediaplayer.h"
#include "mediaplaylist.h"
#include "media.h"
#include <QDebug>
#include "recitation.h"

MediaPlayer::MediaPlayer(QObject *parent) :
  QObject(parent),
  m_list(0),
  m_index(-1),
  m_playing(false),
  m_src(0) {

  m_bin = gst_element_factory_make("playbin2", NULL);

  GstBus *bus = gst_element_get_bus(m_bin);
  gst_bus_add_watch(bus, (GstBusFunc)bus_handler, this);
  gst_object_unref(bus);

  int flags = 0x00000002 // audio
    | 0x00000020; // native audio

  g_object_set(m_bin, "flags", flags, "uri", "appsrc://", NULL);

  g_signal_connect(m_bin, "source-setup", G_CALLBACK(source_setup), this);
}

MediaPlayer::~MediaPlayer() {
  stop();
}

void MediaPlayer::play() {
  if (!m_list) {
    return;
  }

  if (m_playing) {
    return;
  }

  m_playing = true;
  gst_element_set_state(m_bin, GST_STATE_PLAYING);

  emit stateChanged();
}

void MediaPlayer::stop() {
  if (!m_playing) {
    return;
  }

  gst_element_set_state(m_bin, GST_STATE_NULL);
  m_playing = false;

  emit stateChanged();
}

MediaPlaylist *MediaPlayer::playlist() {
  return m_list;
}

void MediaPlayer::setPlaylist(MediaPlaylist *playlist) {
  if (m_list) {
    QObject::disconnect(m_list, SIGNAL(cleared()), this, SLOT(listCleared()));
  }

  m_list = playlist;

  if (m_list) {
    QObject::connect(m_list, SIGNAL(cleared()), this, SLOT(listCleared()));
  }

  m_index = -1;
}

Media *MediaPlayer::media() {
  if (m_index == -1) {
    return 0;
  }

  if (!m_list) {
    return 0;
  }

  QList<Media *> allMedia = m_list->media();
  if (m_index >= allMedia.size()) {
    return 0;
  }

  return allMedia.at(m_index);
}

bool MediaPlayer::isPlaying() {
  return m_playing;
}

void MediaPlayer::setNextIndex() {
  ++m_index;

  Media *m = media();
  if (!m) {
    stop();
    return;
  }

  QByteArray data = m_list->recitation()->data(m);
  if (data.isEmpty()) {
    emit error();
    stop();
  }

  GstBuffer *buffer = gst_buffer_new_and_alloc(data.size());
  memcpy(GST_BUFFER_DATA(buffer), data.constData(), data.size());

  g_signal_emit_by_name(m_src, "push-buffer", buffer, NULL);
  gst_buffer_unref(buffer);

  g_signal_emit_by_name(m_src, "end-of-stream", NULL);

  emit mediaChanged();
}

void MediaPlayer::listCleared() {
  stop();
  m_index = -1;
}

gboolean MediaPlayer::bus_handler(GstBus *bus, GstMessage *message, MediaPlayer *that) {
  Q_UNUSED(bus);
  Q_UNUSED(that);

  switch (GST_MESSAGE_TYPE(message)) {
  case GST_MESSAGE_ERROR: {
    that->stop();
    emit that->error();
    GError *err = NULL;
    gchar *debug;

    gst_message_parse_error(message, &err, &debug);
    qWarning() << "Error" << err->message << ":" << debug;
    g_error_free(err);
    g_free(debug);
  }
    break;

  case GST_MESSAGE_EOS:
    gst_element_set_state(that->m_bin, GST_STATE_READY);
    gst_element_set_state(that->m_bin, GST_STATE_PLAYING);
    break;

  default:
    break;
  }

  return TRUE;
}

void MediaPlayer::source_setup(GstElement *bin, GstElement *src, MediaPlayer *that) {
  Q_UNUSED(bin);

  that->m_src = src;
  that->setNextIndex();
}
