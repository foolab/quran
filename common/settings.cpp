/*
 * Copyright (c) 2011-2020 Mohammed Sameer <msameer@foolab.org>.
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
#include <QStandardPaths>
#include <QDataStream>
#include <QMetaEnum>

#define DEFAULT_TEXT_TYPE          0
#define DEFAULT_FONT_SIZE          36
#define DEFAULT_NUMBER_FORMAT      0
#define DEFAULT_PAGE_NUMBER        0
#define DEFAULT_FULL_SCREEN        false
#define DEFAULT_ORIENTATION        Settings::OrientationPortrait
#define DEFAULT_TRANSLATIONS_HIDDEN   false
#define DEFAULT_FLIP_TO_STOP_RECITATION       false
#define DEFAULT_NIGHT_MODE         false
#define DEFAULT_THEME              "blue"

#define FONT_FAMILY                "Amiri Quran"

#define MIN_TRANSLATION_FONT_SIZE  FONT_MIN_SIZE
#define MAX_TRANSLATION_FONT_SIZE  FONT_MAX_SIZE
#define TRANSLATION_FONT_FAMILY    "Nokia Pure Text"
#define DEFAULT_TRANSLATION_FONT_SIZE       DEFAULT_FONT_SIZE

#define DEFAULT_SEARCH_MATCH_WHOLE_WORDS        true
#define DEFAULT_CENTER_TEXT                     false

#define DEFAULT_RECITATION_MODE             0
Q_DECLARE_METATYPE(QList<uint>);

#if defined(SAILFISH)
#define CONF_FILE "harbour-quran.conf"
#elif defined(ANDROID)
#define CONF_FILE "org.foolab.quran.conf"
#elif defined(DESKTOP)
#define CONF_FILE "quran.conf"
#endif

Settings::Settings(QObject *parent) :
  QObject(parent) {
  qRegisterMetaType<QList<uint> >("QList<uint>");
  qRegisterMetaTypeStreamOperators<QList<uint> >("QList<uint>");

  m_settings = new QSettings(QString("%1%2")
			     .arg(Settings::configurationDir())
			     .arg(CONF_FILE),
			     QSettings::IniFormat);
}

Settings::~Settings() {
  delete m_settings;
  m_settings = 0;
}

QString Settings::dataDir() {
#if defined(ANDROID)
  return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/org.foolab.quran/";
#elif defined(SAILFISH)
  return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/harbour-quran/";
#elif defined(DESKTOP)
  return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/quran/";
#else
#error "I don't know of a data path for this platform"
#endif
}

QString Settings::recitationsDir() const {
  return dataDir() + "/recitations/";
}

QString Settings::translationsDir() const {
  return dataDir() + "/translations/";
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
    m_settings->setValue("General/fontSize", size);
    emit fontSizeChanged();
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

void Settings::setOrientation(Orientation orientation) {
  if (Settings::orientation() != orientation) {
    const QMetaObject &mo = Settings::staticMetaObject;
    int index = mo.indexOfEnumerator("Orientation");
    QMetaEnum metaEnum = mo.enumerator(index);
    QString value = metaEnum.valueToKey(orientation);

    m_settings->setValue("General/orientation", value);
    emit orientationChanged();
  }
}

Settings::Orientation Settings::orientation() const {
  QString value = m_settings->value("General/orientation").toString();
  const QMetaObject &mo = Settings::staticMetaObject;
  int index = mo.indexOfEnumerator("Orientation");
  QMetaEnum metaEnum = mo.enumerator(index);
  bool ok = false;
  int orientation = metaEnum.keyToValue(value.toLatin1().constData(), &ok);
  if (!ok) {
    return DEFAULT_ORIENTATION;
  }

  return static_cast<Settings::Orientation>(orientation);
}

void Settings::setTranslationsHidden(bool hidden) {
  if (areTranslationsHidden() != hidden) {
    m_settings->setValue("General/translationsHidden", hidden);
    emit translationsHiddenChanged();
  }
}

bool Settings::areTranslationsHidden() const {
  return m_settings->value("General/translationsHidden", DEFAULT_TRANSLATIONS_HIDDEN).toBool();
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

QString Settings::configurationDir() {
  QString tpl = QString("%1%2%3%2")
    .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
    .arg(QDir::separator());
#if defined(SAILFISH)
  return tpl.arg("harbour-quran");
#elif defined(ANDROID)
  return tpl.arg("org.foolab.quran");
#else
  return tpl.arg("quran");
#endif
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
  setTranslationsHidden(DEFAULT_TRANSLATIONS_HIDDEN);
}
