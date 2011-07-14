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

public:
  Settings(QObject *parent = 0);
  ~Settings();

  QString fontFamily() const;

  void setFontSize(int size);
  int fontSize() const;

  void setPageNumber(int page);
  int pageNumber() const;

  void setNumberFormat(int format);
  int numberFormat() const;

  void setTextType(int type);
  int textType() const;

  int minFontSize() const;
  int maxFontSize() const;

  void setBookmarks(const QList<uint>& bookmarks);
  QList<uint> bookmarks() const;

  QColor highlightColor() const;

  QString version() const;

  Q_INVOKABLE bool loadFont();

signals:
  void fontSizeChanged();
  void textTypeChanged();
  void numberFormatChanged();
  void pageNumberChanged();

private:
  QSettings *m_settings;
  int m_font;
};

#endif /* SETTINGS_H */
