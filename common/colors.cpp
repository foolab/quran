/*
 * Copyright (c) 2012 Mohammed Sameer <msameer@foolab.org>.
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

#include "colors.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include "settings.h"

Colors::Colors(const QString& path, Settings *settings, QObject *parent) :
  QObject(parent), m_ini(0), m_settings(settings), m_path(path), m_nightMode(false) {

  QObject::connect(m_settings, SIGNAL(themeChanged()), this, SLOT(themeChanged()));
  QObject::connect(m_settings, SIGNAL(nightModeChanged()), this, SLOT(nightModeChanged()));

  themeChanged();
  nightModeChanged();
}

Colors::~Colors() {

}

void Colors::themeChanged() {
  QString id = m_settings->theme();

  if (m_id != id) {
    m_id = id;
    if (m_ini) {
      delete m_ini;
    }

    QString path = QString("%1%2%3%2theme.ini").arg(m_path).arg(QDir::separator()).arg(id);

    m_ini = new QSettings(path, QSettings::IniFormat, this);

    emit colorsChanged();
  }
}

void Colors::nightModeChanged() {
  bool enabled = m_settings->isNightModeEnabled();

  if (m_nightMode != enabled) {
    m_nightMode = enabled;
    emit colorsChanged();
  }
}

QColor Colors::value(const QString& name, const QColor& day, const QColor& night) const {
  if (m_nightMode) {
    return value(QString("night/%1").arg(name), night);
  }
  else {
    return value(QString("day/%1").arg(name), day);
  }
}

QColor Colors::value(const QString& section, const QColor& defaultColor) const {
  QString color = m_ini->value(section).toString();

  if (color.isEmpty()) {
    return defaultColor;
  }

  return QColor(color);
}
