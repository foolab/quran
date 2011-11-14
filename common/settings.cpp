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

#define DEFAULT_TEXT_TYPE          0
#define DEFAULT_FONT_SIZE          36
#define DEFAULT_NUMBER_FORMAT      0
#define DEFAULT_PAGE_NUMBER        0
#define DEFAULT_FULL_SCREEN        false
#define DEFAULT_ORIENTATION        1
#define DEFAULT_TRANSLATION_MODE   0

#define FONT_FAMILY                "Simplified Naskh"
#define FONT_MIN_SIZE              16
#define FONT_MAX_SIZE              48
#define HIGHLIGHT_COLOR            QColor(163, 218, 244)
#define TITLE_COLOR                Qt::black
#define SUBTITLE_COLOR             Qt::black
#define VERSE_COLOR                Qt::black
#define FONT_FILE                  "SimplifiedNaskh.ttf"

#define MIN_TRANSLATION_FONT_SIZE  FONT_MIN_SIZE
#define MAX_TRANSLATION_FONT_SIZE  FONT_MAX_SIZE
#define TRANSLATION_FONT_FAMILY    "Nokia Sans" // TODO: Harmattan
#define DEFAULT_TRANSLATION_FONT_SIZE       DEFAULT_FONT_SIZE

#define DEFAULT_SEARCH_MATCH_WHOLE_WORDS        true
#define DEFAULT_CENTER_TEXT                     false

#define DEFAULT_RECITATION_MODE             0
Q_DECLARE_METATYPE(QList<uint>);

/*!
 * Orientations:
 * 0 = Automatic
 * 1 = Portrait
 * 2 = Landscape
 */
/*!
 * Translation mode:
 * 0 = Off
 * 1 = On
 * 2 = Hidden
 */
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

QColor Settings::verseColor() const {
  return VERSE_COLOR;
}

QColor Settings::titleColor() const {
  return TITLE_COLOR;
}

QColor Settings::subtitleColor() const {
  return SUBTITLE_COLOR;
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

void Settings::setFullScreen(bool fs) {
  if (fullScreen() != fs) {
    m_settings->setValue("General/fullScreen", fs);
    emit fullScreenChanged();
  }
}

bool Settings::fullScreen() const {
  return m_settings->value("General/fullScreen", DEFAULT_FULL_SCREEN).toBool();
}

void Settings::setOrientation(int orientation) {
  int o = qBound(0, orientation, 2);
  if (Settings::orientation() != o) {
    m_settings->setValue("General/orientation", o);
    emit orientationChanged();
  }
}

int Settings::orientation() const {
  return qBound(0, m_settings->value("General/orientation", DEFAULT_ORIENTATION).toInt(), 2);
}

void Settings::setTranslationMode(int mode) {
  int m = qBound(0, mode, 2);

  if (translationMode() != m) {
    m_settings->setValue("General/translationMode", m);
    emit translationModeChanged();
  }
}

int Settings::translationMode() const {
  return qBound(0, m_settings->value("General/translationMode",
				     DEFAULT_TRANSLATION_MODE).toInt(), 2);
}

void Settings::setDefaultTranslation(const QString& id) {
  if (defaultTranslation() != id) {
    m_settings->setValue("General/defaultTranslation", id);
    emit defaultTranslationChanged();
  }
}

QString Settings::defaultTranslation() const {
  return m_settings->value("General/defaultTranslation").toString();
}

int Settings::minTranslationFontSize() const {
  return MIN_TRANSLATION_FONT_SIZE;
}

int Settings::maxTranslationFontSize() const {
  return MAX_TRANSLATION_FONT_SIZE;
}

QString Settings::translationFontFamily() const {
  return TRANSLATION_FONT_FAMILY;
}

int Settings::translationFontSize() const {
  return m_settings->value("General/translationFontSize", DEFAULT_TRANSLATION_FONT_SIZE).toInt();
}

void Settings::setTranslationFontSize(int size) {
  if (size != translationFontSize()) {
    m_settings->setValue("General/translationFontSize", size);
    emit translationFontSizeChanged();
  }
}

bool Settings::searchMatchWholeWords() const {
  return m_settings->value("General/searchMatchWholeWords",
			   DEFAULT_SEARCH_MATCH_WHOLE_WORDS).toBool();
}

void Settings::setSearchMatchWholeWords(bool match) {
  if (match != searchMatchWholeWords()) {
    m_settings->setValue("General/searchMatchWholeWords", match);
    emit searchMatchWholeWordsChanged();
  }
}

bool Settings::centerText() const {
  return m_settings->value("General/centerText", DEFAULT_CENTER_TEXT).toBool();
}

void Settings::setCenterText(bool center) {
  if (centerText() != center) {
    m_settings->setValue("General/centerText", center);
    emit centerTextChanged();
  }
}

int Settings::recitationMode() const {
  return m_settings->value("General/recitationMode", DEFAULT_RECITATION_MODE).toInt();
}

void Settings::setRecitationMode(int mode) {
  if (mode != recitationMode()) {
    m_settings->setValue("General/recitationMode", mode);
    emit recitationModeChanged();
  }
}

void Settings::setDefaultRecitation(const QString& id) {
  if (defaultRecitation() != id) {
    m_settings->setValue("General/defaultRecitation", id);
    emit defaultRecitationChanged();
  }
}

QString Settings::defaultRecitation() const {
  return m_settings->value("General/defaultRecitation").toString();
}

void Settings::reset() {
  setFullScreen(DEFAULT_FULL_SCREEN);
  setOrientation(DEFAULT_ORIENTATION);
  setFontSize(DEFAULT_FONT_SIZE);
  setNumberFormat(DEFAULT_NUMBER_FORMAT);
  setTextType(DEFAULT_TEXT_TYPE);
  setTranslationFontSize(DEFAULT_TRANSLATION_FONT_SIZE);
  setCenterText(DEFAULT_CENTER_TEXT);
  setRecitationMode(DEFAULT_RECITATION_MODE);
}
