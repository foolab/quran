/*
 * Copyright (c) 2020 Mohammed Sameer <msameer@foolab.org>.
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

#include "mediastate.h"
#include "mediaplayerconfig.h"
#include <QSettings>
#include <QDir>
#include "settings.h"

#define CONF_FILE "media-state.conf"

MediaState::MediaState(QObject *parent) :
  QObject(parent),
  m_hasChanges(false) {
  m_settings = new QSettings(QString("%1%2%3")
			     .arg(Settings::configurationDir())
			     .arg(QDir::separator())
			     .arg(CONF_FILE),
			     QSettings::IniFormat);
}

MediaState::~MediaState() {
  save();

  delete m_settings;
  m_settings = 0;
}

void MediaState::setPosition(int position) {
  m_settings->setValue("media/position", position);
  m_hasChanges = true;

  QMetaObject::invokeMethod(this, "save", Qt::QueuedConnection);
}

int MediaState::position() {
  return m_settings->value("media/position", -1).toInt();
}

void MediaState::setConfig(const MediaPlayerConfig& config) {
  m_settings->setValue("media/config", config.toByteArray());
  m_hasChanges = true;

  QMetaObject::invokeMethod(this, "save", Qt::QueuedConnection);
}

MediaPlayerConfig MediaState::config() {
  QByteArray conf = m_settings->value("media/config").toByteArray();
  if (!conf.isEmpty()) {
    return MediaPlayerConfig::fromByteArray(conf);
  }

  return MediaPlayerConfig();
}

void MediaState::save() {
  if (m_hasChanges) {
    m_settings->sync();
    m_hasChanges = false;
  }
}

void MediaState::clear() {
  m_settings->clear();
  m_hasChanges = true;
  save();
}
