/*
 * Copyright (c) 2011-2012 Mohammed Sameer <msameer@foolab.org>.
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
  Q_PROPERTY(int minFontSize READ minFontSize CONSTANT);
  Q_PROPERTY(int maxFontSize READ maxFontSize CONSTANT);
  Q_PROPERTY(QString translationFontFamily READ translationFontFamily CONSTANT);
  Q_PROPERTY(int minTranslationFontSize READ minTranslationFontSize CONSTANT);
  Q_PROPERTY(int maxTranslationFontSize READ maxTranslationFontSize CONSTANT);
  Q_PROPERTY(int translationFontSize READ translationFontSize WRITE setTranslationFontSize NOTIFY translationFontSizeChanged);

  Q_PROPERTY(QString version READ version CONSTANT);
  Q_PROPERTY(bool fontLoaded READ isFontLoaded CONSTANT);
  Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged);
  Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged);
  Q_PROPERTY(int translationMode READ translationMode WRITE setTranslationMode NOTIFY translationModeChanged);
  Q_PROPERTY(QString defaultTranslation READ defaultTranslation WRITE setDefaultTranslation NOTIFY defaultTranslationChanged);

  Q_PROPERTY(bool searchMatchWholeWords READ searchMatchWholeWords WRITE setSearchMatchWholeWords NOTIFY searchMatchWholeWordsChanged);

  Q_PROPERTY(bool centerText READ centerText WRITE setCenterText NOTIFY centerTextChanged);

  Q_PROPERTY(int recitationMode READ recitationMode WRITE setRecitationMode NOTIFY recitationModeChanged);
  Q_PROPERTY(QString defaultRecitation READ defaultRecitation WRITE setDefaultRecitation NOTIFY defaultRecitationChanged);
  Q_PROPERTY(bool flipToStopRecitation READ flipToStopRecitation WRITE setFlipToStopRecitation NOTIFY flipToStopRecitationChanged);

  Q_PROPERTY(bool nightMode READ isNightModeEnabled WRITE setNightModeEnabled NOTIFY nightModeChanged);

public:
  Settings(QObject *parent = 0);
  ~Settings();

  QString fontFamily() const;

  int fontSize() const;

  void setPageNumber(int page);
  int pageNumber() const;

  int numberFormat() const;

  int textType() const;

  int minFontSize() const;
  int maxFontSize() const;

  int minTranslationFontSize() const;
  int maxTranslationFontSize() const;
  int translationFontSize() const;
  QString translationFontFamily() const;

  void setBookmarks(const QList<uint>& bookmarks);
  QList<uint> bookmarks() const;

  QString version() const;

  void loadFont();

  bool isFontLoaded() const;

  void setFullScreen(bool fs);
  bool fullScreen() const;

  void setOrientation(int orientation);
  int orientation() const;

  void setTranslationMode(int mode);
  int translationMode() const;

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
  void translationModeChanged();
  void defaultTranslationChanged();
  void translationFontSizeChanged();
  void searchMatchWholeWordsChanged();
  void centerTextChanged();
  void recitationModeChanged();
  void defaultRecitationChanged();
  void flipToStopRecitationChanged();
  void nightModeChanged();

private:
  QSettings *m_settings;
  int m_font;
};

#endif /* SETTINGS_H */
