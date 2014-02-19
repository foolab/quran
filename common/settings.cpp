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

#include "settings.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QFontDatabase>
#ifdef SAILFISH
#include <QStandardPaths>
#endif

#define DEFAULT_TEXT_TYPE          0
#define DEFAULT_FONT_SIZE          36
#define DEFAULT_NUMBER_FORMAT      0
#define DEFAULT_PAGE_NUMBER        0
#define DEFAULT_FULL_SCREEN        false
#define DEFAULT_ORIENTATION        1
#define DEFAULT_TRANSLATION_MODE   0
#define DEFAULT_FLIP_TO_STOP_RECITATION       true
#define DEFAULT_NIGHT_MODE         false
#define DEFAULT_THEME              "blue"
#define DEFAULT_ONLINE_RECITATIONS true

#define FONT_FAMILY                "Simplified Naskh"
#ifdef SAILFISH
#define FONT_MIN_SIZE              22
#define FONT_MAX_SIZE              54
#else
#define FONT_MIN_SIZE              16
#define FONT_MAX_SIZE              48
#endif

#define MIN_TRANSLATION_FONT_SIZE  FONT_MIN_SIZE
#define MAX_TRANSLATION_FONT_SIZE  FONT_MAX_SIZE
#define TRANSLATION_FONT_FAMILY    "Nokia Pure Text"
#define DEFAULT_TRANSLATION_FONT_SIZE       DEFAULT_FONT_SIZE

#define DEFAULT_SEARCH_MATCH_WHOLE_WORDS        true
#define DEFAULT_CENTER_TEXT                     false

#define DEFAULT_RECITATION_MODE             0
Q_DECLARE_METATYPE(QList<uint>);

#ifdef SAILFISH
#define CONF_FILE "harbour-quran.conf"
#else
#define USER_DIR "/home/user/MyDocs/.n9-quran/"
#define CONF_FILE "quran.conf"
#endif

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
Settings::Settings(QObject *parent) : QObject(parent) {
  qRegisterMetaType<QList<uint> >("QList<uint>");
  qRegisterMetaTypeStreamOperators<QList<uint> >("QList<uint>");

#ifdef SAILFISH
  m_settings = new QSettings(QString("%1%2harbour-quran%2%3")
			     .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
			     .arg(QDir::separator()).arg(CONF_FILE),
			     QSettings::IniFormat);
#else
  m_settings = new QSettings(QString("%1%2.config%2%3")
			     .arg(QDir::homePath()).arg(QDir::separator()).arg(CONF_FILE),
			     QSettings::IniFormat);
#endif
}

Settings::~Settings() {
  delete m_settings;
  m_settings = 0;
}

QString Settings::dataDir() const {
#ifdef SAILFISH
  static QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
#else
  static QString dir = USER_DIR;
#endif

  return dir;
}

QString Settings::recitationsDir() const {
  return dataDir() + "/recitations/";
}

QString Settings::translationsDir() const {
  return dataDir() + "/translations/";
}

QString Settings::recitationsSubDir() const {
  return ".n9-quran/recitations/";
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

QString Settings::version() const {
  return VERSION;
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

void Settings::setFlipToStopRecitation(bool enabled) {
  if (enabled != flipToStopRecitation()) {
    m_settings->setValue("General/flipToStopRecitation", enabled);
    emit flipToStopRecitationChanged();
  }
}

bool Settings::flipToStopRecitation() {
  return m_settings->value("General/flipToStopRecitation",
			   DEFAULT_FLIP_TO_STOP_RECITATION).toBool();
}

void Settings::setNightModeEnabled(bool enabled) {
  if (enabled != isNightModeEnabled()) {
    m_settings->setValue("General/nightMode", enabled);
    emit nightModeChanged();
  }
}

bool Settings::isNightModeEnabled() {
  return m_settings->value("General/nightMode", DEFAULT_NIGHT_MODE).toBool();
}

void Settings::setTheme(const QString& theme) {
  if (theme != Settings::theme()) {
    m_settings->setValue("General/theme", theme);
    emit themeChanged();
  }
}

QString Settings::theme() {
  return m_settings->value("General/theme", DEFAULT_THEME).toString();
}

void Settings::setOnlineRecitationsEnabled(bool enabled) {
  if (areOnlineRecitationsEnabled() != enabled) {
    m_settings->setValue("General/onlineRecitations", enabled);
    emit onlineRecitationsChanged();
  }
}

bool Settings::areOnlineRecitationsEnabled() const {
  return m_settings->value("General/onlineRecitations", DEFAULT_ONLINE_RECITATIONS).toBool();
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
  setFlipToStopRecitation(DEFAULT_FLIP_TO_STOP_RECITATION);
  setNightModeEnabled(DEFAULT_NIGHT_MODE);
  setTheme(DEFAULT_THEME);
  setOnlineRecitationsEnabled(DEFAULT_ONLINE_RECITATIONS);
}
