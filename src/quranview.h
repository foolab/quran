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

#ifndef QURAN_VIEW_H
#define QURAN_VIEW_H

#include <QDeclarativeItem>
#include <QTextFragment>
#include <QColor>

class DataProvider;
class QTextDocument;
class QTextCursor;
class Fragment;
class Bookmarks;
class NumberFormatter;

class QuranView : public QDeclarativeItem {
  Q_OBJECT
  Q_PROPERTY(int page READ page WRITE setPage);
  Q_PROPERTY(DataProvider * dataProvider READ dataProvider WRITE setDataProvider);
  Q_PROPERTY(Bookmarks * bookmarks READ bookmarks WRITE setBookmarks);
  Q_PROPERTY(NumberFormatter * formatter READ formatter WRITE setFormatter);
  Q_PROPERTY(QFont font READ font WRITE setFont);
  Q_PROPERTY(qreal margin READ margin WRITE setMargin);
  Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor);

public:
  QuranView(QDeclarativeItem *parent = 0);
  ~QuranView();

  void setPage(int page);
  int page();

  void setFont(const QFont& font);
  QFont font();

  void setDataProvider(DataProvider *data);
  DataProvider *dataProvider();

  void setMargin(qreal margin);
  qreal margin();

  void setBookmarks(Bookmarks *bookmarks);
  Bookmarks *bookmarks();

  void setFormatter(NumberFormatter *formatter);
  NumberFormatter *formatter();

  void setHighlightColor(const QColor& color);
  QColor highlightColor() const;

  virtual void componentComplete();
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		     QWidget *widget = 0);

  Q_INVOKABLE QVariant bookmarkId(int x, int y);
  Q_INVOKABLE QString textForPosition(int x, int y);

public slots:
  void mouseClicked(int x, int y);
  void populate();
  void selectRequested(int sura, int aya);

protected:
  virtual void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

signals:
  void lineVisibilityRequested(qreal upper, qreal lower);

private:
  void updateLayout();

  void clearSelection();

  void begin(const QList<Fragment>& frags);
  void addFragment(QTextCursor& cursor, const Fragment& frag);
  void end(QTextCursor& cursor, const QList<Fragment>& frags);

  int m_page;
  DataProvider *m_data;
  Bookmarks *m_bookmarks;
  NumberFormatter *m_formatter;
  QColor m_color;

  QTextDocument *m_doc;

  QTextFragment m_highlighted;
};

#endif /* QURAN_VIEW_H */
