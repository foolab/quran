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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPoint>

class QSettings;

class Settings : public QObject {
  Q_OBJECT

  Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged);
  Q_PROPERTY(int pageNumber READ pageNumber WRITE setPageNumber NOTIFY pageNumberChanged);
  Q_PROPERTY(int textType READ textType WRITE setTextType NOTIFY textTypeChanged);
  Q_PROPERTY(int numberFormat READ numberFormat WRITE setNumberFormat NOTIFY numberFormatChanged);
  Q_PROPERTY(QString fontFamily READ fontFamily CONSTANT);
  Q_PROPERTY(QString translationFontFamily READ translationFontFamily CONSTANT);
  Q_PROPERTY(int translationFontSize READ translationFontSize WRITE setTranslationFontSize NOTIFY translationFontSizeChanged);

  Q_PROPERTY(QString version READ version CONSTANT);
  Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged);
  Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged);
  Q_PROPERTY(bool translationsHidden READ areTranslationsHidden WRITE setTranslationsHidden NOTIFY translationsHiddenChanged);
  Q_PROPERTY(QString defaultTranslation READ defaultTranslation WRITE setDefaultTranslation NOTIFY defaultTranslationChanged);

  Q_PROPERTY(bool searchMatchWholeWords READ searchMatchWholeWords WRITE setSearchMatchWholeWords NOTIFY searchMatchWholeWordsChanged);

  Q_PROPERTY(bool centerText READ centerText WRITE setCenterText NOTIFY centerTextChanged);

  Q_PROPERTY(int recitationMode READ recitationMode WRITE setRecitationMode NOTIFY recitationModeChanged);
  Q_PROPERTY(QString defaultRecitation READ defaultRecitation WRITE setDefaultRecitation NOTIFY defaultRecitationChanged);
  Q_PROPERTY(bool flipToStopRecitation READ flipToStopRecitation WRITE setFlipToStopRecitation NOTIFY flipToStopRecitationChanged);

  Q_PROPERTY(bool nightMode READ isNightModeEnabled WRITE setNightModeEnabled NOTIFY nightModeChanged);
  Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged);
  Q_PROPERTY(QString recitationsDir READ recitationsDir CONSTANT);
  Q_PROPERTY(QString translationsDir READ translationsDir CONSTANT);
  Q_ENUMS(Orientation);

public:
  typedef enum {
    OrientationAutomatic = 0,
    OrientationPortrait = 1,
    OrientationLandscape = 2,
  } Orientation;

  Settings(QObject *parent = 0);
  ~Settings();

  static QString dataDir();

  QString fontFamily() const;

  int fontSize() const;

  void setPageNumber(int page);
  int pageNumber() const;

  int numberFormat() const;

  int textType() const;

  int translationFontSize() const;
  QString translationFontFamily() const;

  void setBookmarks(const QList<uint>& bookmarks);
  QList<uint> bookmarks() const;

  QString version() const;

  bool isFontLoaded() const;

  void setFullScreen(bool fs);
  bool fullScreen() const;

  void setOrientation(Orientation orientation);
  Orientation orientation() const;

  void setTranslationsHidden(bool hidden);
  bool areTranslationsHidden() const;

  void setDefaultTranslation(const QString& id);
  QString defaultTranslation() const;

  bool searchMatchWholeWords() const;
  void setSearchMatchWholeWords(bool match);

  bool centerText() const;
  void setCenterText(bool center);

  int recitationMode() const;
  void setRecitationMode(int mode);

  void setDefaultRecitation(const QString& id);
  QString defaultRecitation() const;

  void setFlipToStopRecitation(bool enabled);
  bool flipToStopRecitation();

  void setNightModeEnabled(bool enabled);
  bool isNightModeEnabled();

  void setTheme(const QString& theme);
  QString theme();

  QString recitationsDir() const;
  QString translationsDir() const;

  static QString configurationDir();

public slots:
  void reset();
  void setFontSize(int size);
  void setNumberFormat(int format);
  void setTextType(int type);
  void setTranslationFontSize(int size);

signals:
  void fontSizeChanged();
  void textTypeChanged();
  void numberFormatChanged();
  void pageNumberChanged();
  void yChanged();
  void fullScreenChanged();
  void orientationChanged();
  void translationsHiddenChanged();
  void defaultTranslationChanged();
  void translationFontSizeChanged();
  void searchMatchWholeWordsChanged();
  void centerTextChanged();
  void recitationModeChanged();
  void defaultRecitationChanged();
  void flipToStopRecitationChanged();
  void nightModeChanged();
  void themeChanged();

private:
  QSettings *m_settings;
};

#endif /* SETTINGS_H */
