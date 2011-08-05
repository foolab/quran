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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QColor>
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
  Q_PROPERTY(QColor highlightColor READ highlightColor CONSTANT);
  Q_PROPERTY(QString version READ version CONSTANT);
  Q_PROPERTY(bool fontLoaded READ isFontLoaded CONSTANT);
  Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged);
  Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged);

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

  void setBookmarks(const QList<uint>& bookmarks);
  QList<uint> bookmarks() const;

  QColor highlightColor() const;

  QString version() const;

  void loadFont();

  bool isFontLoaded() const;

  void setPosition(const QPoint& pos);
  QPoint position() const;

  int y() const;

public slots:
  void setFontSize(int size);
  void setNumberFormat(int format);
  void setTextType(int type);
  void setY(int y);

signals:
  void fontSizeChanged();
  void textTypeChanged();
  void numberFormatChanged();
  void pageNumberChanged();
  void positionChanged();
  void yChanged();

private:
  QSettings *m_settings;
  int m_font;
};

#endif /* SETTINGS_H */
