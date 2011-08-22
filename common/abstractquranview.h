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

#ifndef ABSTRACT_QURAN_VIEW_H
#define ABSTRACT_QURAN_VIEW_H

#include <QObject>
#include <QTextFragment>
#include <QColor>
#include "position.h"

class DataProvider;
class QTextDocument;
class QTextCursor;
class Fragment;
class Bookmarks;
class NumberFormatter;

class AbstractQuranView {
public:
  AbstractQuranView(QTextDocument *doc);
  ~AbstractQuranView();

  void setFont(const QFont& font);
  QFont font();

  void setDataProvider(DataProvider *data);
  DataProvider *dataProvider();

  void setBookmarks(Bookmarks *bookmarks);
  Bookmarks *bookmarks();

  void setFormatter(NumberFormatter *formatter);
  NumberFormatter *formatter();

  void setHighlightColor(const QColor& color);
  QColor highlightColor() const;

  void setTitleColor(const QColor& color);
  QColor titleColor() const;

  void setSubtitleColor(const QColor& color);
  QColor subtitleColor() const;

  void setVerseColor(const QColor& color);
  QColor verseColor() const;

  void setShowTranslation(bool show);
  bool showTranslation() const;

  void populate(int page);

  Position position(int x, int y);

  QLineF select(const Position& position);
  QLineF position(const Position& position);

  void clearSelection();

protected:
  QTextDocument *m_doc;
  DataProvider *m_data;
  Bookmarks *m_bookmarks;
  NumberFormatter *m_formatter;

private:
  QLineF position(const Position& position, bool highlight);

  void begin(const QList<Fragment>& frags);
  void addFragment(QTextCursor& cursor, const Fragment& frag);
  void end(QTextCursor& cursor, const QList<Fragment>& frags);

  QColor m_highlightColor;
  QColor m_titleColor;
  QColor m_subtitleColor;
  QColor m_verseColor;

  QTextFragment m_highlighted;

  bool m_showTranslation;
};

#endif /* ABSTRACT_QURAN_VIEW_H */
