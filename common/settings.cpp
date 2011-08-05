/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
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

#include "settings.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QFontDatabase>

#define DEFAULT_TEXT_TYPE      0
#define DEFAULT_FONT_SIZE      36
#define DEFAULT_NUMBER_FORMAT  0
#define DEFAULT_PAGE_NUMBER    0
#define DEFAULT_Y              0

#define FONT_FAMILY            "me_quran"
#define FONT_MIN_SIZE          16
#define FONT_MAX_SIZE          48
#define HIGHLIGHT_COLOR        QColor(163, 218, 244)

#define FONT_FILE              "me_quran_volt_newmet.ttf"

Q_DECLARE_METATYPE(QList<uint>);

Settings::Settings(QObject *parent) : QObject(parent), m_font(-1) {
  qRegisterMetaType<QList<uint> >("QList<uint>");
  qRegisterMetaTypeStreamOperators<QList<uint> >("QList<uint>");

  m_settings = new QSettings(QString("%1%2.config%2quran.conf")
			     .arg(QDir::homePath()).arg(QDir::separator()),
			     QSettings::IniFormat);
}

Settings::~Settings() {
  delete m_settings;
  m_settings = 0;
}

QString Settings::fontFamily() const {
  return FONT_FAMILY;
}

int Settings::pageNumber() const {
  return m_settings->value("General/pageNumber", DEFAULT_PAGE_NUMBER).toInt();
}

void Settings::setPageNumber(int page) {
  if (page != pageNumber()) {
    m_settings->setValue("General/pageNumber", page);
    emit pageNumberChanged();
  }
}

void Settings::setFontSize(int size) {
  if (size != fontSize()) {
    if (size >= minFontSize() && size <= maxFontSize()) {
      m_settings->setValue("General/fontSize", size);
      emit fontSizeChanged();
    }
  }
}

int Settings::fontSize() const {
  return m_settings->value("General/fontSize", DEFAULT_FONT_SIZE).toInt();
}

int Settings::numberFormat() const {
  return m_settings->value("General/numberFormat", DEFAULT_NUMBER_FORMAT).toInt();
}

void Settings::setNumberFormat(int format) {
  if (format != numberFormat()) {
    m_settings->setValue("General/numberFormat", format);
    emit numberFormatChanged();
  }
}

void Settings::setTextType(int type) {
  if (type != textType()) {
    m_settings->setValue("General/textType", type);
    emit textTypeChanged();
  }
}

int Settings::textType() const {
  return m_settings->value("General/textType", DEFAULT_TEXT_TYPE).toInt();
}

int Settings::minFontSize() const {
  return FONT_MIN_SIZE;
}

int Settings::maxFontSize() const {
  return FONT_MAX_SIZE;
}

void Settings::setBookmarks(const QList<uint>& bookmarks) {
  m_settings->setValue("General/bookmarks", QVariant::fromValue<QList<uint> >(bookmarks));
}

QList<uint> Settings::bookmarks() const {
  return m_settings->value("General/bookmarks").value<QList<uint> >();
}

QColor Settings::highlightColor() const {
  return HIGHLIGHT_COLOR;
}

QString Settings::version() const {
  return VERSION;
}

void Settings::loadFont() {
  if (m_font != -1) {
    return;
  }

  m_font = QFontDatabase::addApplicationFont(DATA_DIR FONT_FILE);
}

bool Settings::isFontLoaded() const {
  return m_font != -1;
}

void Settings::setPosition(const QPoint& pos) {
  if (pos != position()) {
    m_settings->setValue("General/position", pos);
    emit positionChanged();
  }
}

QPoint Settings::position() const {
  return m_settings->value("General/position", QPoint(0, 0)).toPoint();
}

int Settings::y() const {
  return m_settings->value("General/y", DEFAULT_Y).toInt();
}

void Settings::setY(int y) {
  if (y != Settings::y()) {
    m_settings->setValue("General/y", y);
    emit yChanged();
  }
}
