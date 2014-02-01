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

MediaPlayer::MediaPlayer(QObject *parent) :
  QObject(parent),
  m_list(0),
  m_index(-1) {

  m_bin = gst_element_factory_make("playbin2", NULL);

  GstBus *bus = gst_element_get_bus(m_bin);
  gst_bus_add_watch(bus, (GstBusFunc)bus_handler, this);
  gst_object_unref(bus);

  int flags = 0x00000002 // audio
    | 0x00000020; // native audio

  g_object_set(m_bin, "flags", flags, NULL);

  g_signal_connect(m_bin, "about-to-finish", G_CALLBACK(queue_next_uri), this);
}

MediaPlayer::~MediaPlayer() {
  stop();
}

void MediaPlayer::play() {
  if (!m_list) {
    return;
  }

  setNextIndex();

  gst_element_set_state(m_bin, GST_STATE_PLAYING);
  isPlaying();

  emit stateChanged();
}

void MediaPlayer::stop() {
  if (!isPlaying()) {
    return;
  }

  gst_element_set_state(m_bin, GST_STATE_NULL);
  isPlaying();

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
  GstState state;
  GstStateChangeReturn err = gst_element_get_state(m_bin, &state, 0, GST_CLOCK_TIME_NONE);

  if (err == GST_STATE_CHANGE_FAILURE || state != GST_STATE_PLAYING) {
    return false;
  }

  return true;
}

void MediaPlayer::setNextIndex() {
  ++m_index;
  Media *m = media();
  if (!m) {
    return;
  }

  g_object_set (m_bin, "uri", m->url().toString().toLocal8Bit().constData(), NULL);

  qDebug() << "playing url" << m->url();

  emit mediaChanged();
}

void MediaPlayer::listCleared() {
  stop();
  m_index = -1;
}

void MediaPlayer::queue_next_uri(GstElement* elem, MediaPlayer *that) {
  Q_UNUSED(elem);

  that->setNextIndex();
}

gboolean MediaPlayer::bus_handler(GstBus *bus, GstMessage *message, MediaPlayer *that) {
  Q_UNUSED(bus);
  Q_UNUSED(that);

  switch (GST_MESSAGE_TYPE(message)) {
  case GST_MESSAGE_ERROR: {
    GError *err = NULL;
    gchar *debug;

    gst_message_parse_error(message, &err, &debug);
    qDebug() << "Error" << err->message << ":" << debug;
    g_error_free(err);
    g_free(debug);
  }
    break;

  case GST_MESSAGE_WARNING: {
    GError *err = NULL;
    gchar *debug;

    gst_message_parse_warning(message, &err, &debug);
    qDebug() << "Warning" << err->message << ":" << debug;

    g_error_free(err);
    g_free(debug);
  }
    break;

  case GST_MESSAGE_INFO: {
    GError *err = NULL;
    gchar *debug;

    gst_message_parse_info(message, &err, &debug);

    qDebug() << "Info" << err->message << ":" << debug;

    g_error_free(err);
    g_free(debug);
  }
    break;

  case GST_MESSAGE_EOS:
    qDebug() << "EOS";
    that->stop();
    break;

  default:
    break;
  }

  return TRUE;
}
