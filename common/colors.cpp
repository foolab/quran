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

Colors::Colors(const QString& path, QObject *parent) : QObject(parent), m_ini(0),
						       m_path(path), m_nightMode(false) {

}

Colors::~Colors() {

}

void Colors::setId(const QString& id) {
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

QString Colors::id() const {
  return m_id;
}

void Colors::setNightMode(bool enabled) {
  if (m_nightMode != enabled) {
    m_nightMode = enabled;
    emit colorsChanged();
  }
}

bool Colors::nightMode() const {
  return m_nightMode;
}

QColor Colors::textColor() const {
  return value("textColor", Qt::black, Qt::white);
}

QColor Colors::backgroundColor() const {
  return value("backgroundColor", Qt::white, Qt::black);
}

QColor Colors::verseColor() const {
  return value("verseColor", Qt::black, Qt::white);
}

QColor Colors::titleColor() const {
  return value("titleColor", Qt::white, Qt::black);
}

QColor Colors::subtitleColor() const {
  return value("subtitleColor", Qt::white, Qt::black);
}

QColor Colors::highlightColor() const {
  return value("highlightColor", Qt::red, Qt::red);
}

QColor Colors::faderColor() const {
  return value("faderColor", "steelblue", Qt::black);
}

QColor Colors::sectionColor() const {
  return value("sectionColor", QColor(163, 218, 244), QColor(163, 218, 244));
}

QColor Colors::value(const QString& name, const QColor& day, const QColor& night) const {
  if (m_nightMode) {
    return m_ini->value(QString("night/%1").arg(name), night).value<QColor>();
  }
  else {
    return m_ini->value(QString("day/%1").arg(name), day).value<QColor>();
  }
}
