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

#include "abstractquranview.h"
#include <QDebug>
#include <QTextDocument>
#include "dataprovider.h"
#include <QTextCursor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QAbstractTextDocumentLayout>
#include "bookmarks.h"
#include <QTextBlock>
#include "numberformatter.h"

#define BOOKMARKS_PROPERTY QTextFormat::UserProperty

AbstractQuranView::AbstractQuranView(QTextDocument *doc) :
  m_data(0), m_bookmarks(0), m_formatter(0),
  m_doc(doc) {

  m_doc->setDocumentMargin(0);
  m_doc->setUndoRedoEnabled(false);
}

AbstractQuranView::~AbstractQuranView() {
  m_data = 0;
}

void AbstractQuranView::setFont(const QFont& font) {
  m_doc->setDefaultFont(font);
}

QFont AbstractQuranView::font() {
  return m_doc->defaultFont();
}

void AbstractQuranView::setDataProvider(DataProvider *data) {
  m_data = data;
}

DataProvider *AbstractQuranView::dataProvider() {
  return m_data;
}

void AbstractQuranView::setBookmarks(Bookmarks *bookmarks) {
  m_bookmarks = bookmarks;
}

Bookmarks *AbstractQuranView::bookmarks() {
  return m_bookmarks;
}

void AbstractQuranView::setFormatter(NumberFormatter *formatter) {
  m_formatter = formatter;
}

NumberFormatter *AbstractQuranView::formatter() {
  return m_formatter;
}

void AbstractQuranView::setHighlightColor(const QColor& color) {
  m_color = color;
}

QColor AbstractQuranView::highlightColor() const {
  return m_color;
}

void AbstractQuranView::populate(int page) {
  Page p = m_data->pageFromIndex(page);

  QList<Fragment> frags = p.fragments();

  begin(frags);

  QTextCursor c(m_doc);

  foreach (const Fragment& frag, frags) {
    addFragment(c, frag);
  }

  end(c, frags);
}

#if 0
void AbstractQuranView::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  if (isComponentComplete() && newGeometry.width() != oldGeometry.width()) {
    updateLayout();
  }

  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
}

void AbstractQuranView::updateLayout() {
  m_doc->setTextWidth(width());
}

void AbstractQuranView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		      QWidget *widget) {

  Q_UNUSED(widget);

  // TODO: is this needed ?
  painter->save();

  QAbstractTextDocumentLayout::PaintContext ctx;
  if (option->exposedRect.isValid()) {
    qreal margin = m_doc->documentMargin();
    ctx.clip = option->exposedRect.adjusted(margin, margin, -margin, -margin);
  }

  m_doc->documentLayout()->draw(painter, ctx);

  painter->restore();
}

void AbstractQuranView::populate() {

}
#endif

void AbstractQuranView::begin(const QList<Fragment>& frags) {
  Q_UNUSED(frags);

  m_doc->clear();
}

void AbstractQuranView::addFragment(QTextCursor& cursor, const Fragment& frag) {
  Sura s = m_data->sura(frag.sura());

  QTextBlockFormat centerFormat;
  centerFormat.setAlignment(Qt::AlignHCenter);

  if (frag.start() == 0) {
    cursor.insertBlock(centerFormat);
    cursor.insertText(m_data->fullSuraName(frag.sura()), QTextCharFormat());

    if (s.hasBasmala()) {
      cursor.insertBlock(centerFormat);
      cursor.insertText(m_data->basmala(), QTextCharFormat());
    }

    cursor.insertBlock(QTextBlockFormat());
  }

  QTextCharFormat fmt;

  // Ayat:
  QStringList text = m_data->text(frag);
  int sura = frag.sura();

  for (int x = 0; x < text.size(); x++) {
    int ayaNumber = x + frag.start();

    QString aya = QString("%1 (%2)").arg(text.at(x)).arg(m_formatter->number(ayaNumber + 1));
    fmt.setProperty(BOOKMARKS_PROPERTY, m_bookmarks->serialize(sura, ayaNumber));
    cursor.insertText(aya, fmt);

    if (x + 1 != frag.size()) {
      cursor.insertText(" ", QTextCharFormat());
    }
  }
}

void AbstractQuranView::end(QTextCursor& cursor, const QList<Fragment>& frags) {
  if (!frags.isEmpty() && frags.at(0).start() == 0) {
    // Seems we will get an empty line at the beginning
    // of the document if we start it with a block.
    // QTextCursor::movePosition(QTextCursor::Start) before the first insertion
    // doesn't seem to work :|
    cursor.setPosition(0);
    cursor.deleteChar();
  }

  // TODO: account for margin ?
}

#if 0
QString AbstractQuranView::textForPosition(int x, int y) {
  int pos = m_doc->documentLayout()->hitTest(QPointF(x, y), Qt::FuzzyHit);
  if (pos == -1) {
    return QString();
  }

  for (QTextBlock block = m_doc->begin(); block != m_doc->end(); block = block.next()) {
    for (QTextBlock::Iterator it = block.begin(); !(it.atEnd()); ++it) {

      QTextFragment frag(it.fragment());

      int start = frag.position();
      int end = start + frag.length();

      if (pos >= start && pos <= end && frag.length() != 1) {
	return frag.text();
      }
    }
  }

  return QString();
}

QVariant AbstractQuranView::bookmarkId(int x, int y) {
  int pos = m_doc->documentLayout()->hitTest(QPointF(x, y), Qt::FuzzyHit);
  if (pos == -1) {
    return QVariant();
  }

  for (QTextBlock block = m_doc->begin(); block != m_doc->end(); block = block.next()) {
    for (QTextBlock::Iterator it = block.begin(); !(it.atEnd()); ++it) {

      QTextFragment frag(it.fragment());

      int start = frag.position();
      int end = start + frag.length();

      if (pos >= start && pos <= end && frag.length() != 1) {
	if (!frag.charFormat().hasProperty(BOOKMARKS_PROPERTY)) {
	  // Only select ayat
	  return QVariant();
	}

	return frag.charFormat().property(BOOKMARKS_PROPERTY);
      }
    }
  }

  return QVariant();
}
#endif

QLineF AbstractQuranView::position(const Position& position) {
  return AbstractQuranView::position(position, false);
}

QLineF AbstractQuranView::select(const Position& position) {
  return AbstractQuranView::position(position, true);
}

QLineF AbstractQuranView::position(const Position& position, bool highlight) {
  uint bookmark = m_bookmarks->serialize(position.sura(), position.aya());

  qreal height = 0;

  if (highlight) {
    clearSelection();
  }

  QTextCursor c(m_doc);

  for (QTextBlock block = m_doc->begin(); block != m_doc->end(); block = block.next()) {
    for (QTextBlock::Iterator it = block.begin(); !(it.atEnd()); ++it) {

      QTextFragment frag(it.fragment());

      if (frag.charFormat().hasProperty(BOOKMARKS_PROPERTY) &&
	  frag.charFormat().property(BOOKMARKS_PROPERTY).value<uint>() == bookmark) {

	int start = frag.position();
	int end = start + frag.length();

	if (highlight) {
	  // Select
	  c.setPosition(start, QTextCursor::MoveAnchor);
	  c.setPosition(end, QTextCursor::KeepAnchor);

	  // Apply background color
	  QTextCharFormat fmt;
	  fmt.setBackground(m_color);
	  c.mergeCharFormat(fmt);

	  // Update selection:
	  m_highlighted = frag;

	  // deselect
	  c.clearSelection();
	}

	// calculate line position
        QRectF startRect = block.layout()->lineForTextPosition(start - block.position()).naturalTextRect();
        qreal upper = height + startRect.y();
	upper += m_doc->documentMargin();

        QRectF endRect = block.layout()->lineForTextPosition(end - block.position()).naturalTextRect();
        qreal lower = height + endRect.y() + endRect.height();
	lower += m_doc->documentMargin();

	return QLineF(0, upper, 0, lower);;
      }
    }

    height += m_doc->documentLayout()->blockBoundingRect(block).height();
  }

  return QLineF();
}

void AbstractQuranView::clearSelection() {
  QTextCursor c(m_doc);

  if (m_highlighted.isValid()) {
    QTextCharFormat fmt(m_highlighted.charFormat());
    c.setPosition(m_highlighted.position(), QTextCursor::MoveAnchor);
    c.setPosition(m_highlighted.position() + m_highlighted.length(), QTextCursor::KeepAnchor);

    fmt.clearBackground();
    c.setCharFormat(fmt);
    c.clearSelection();
  }
}

Position AbstractQuranView::position(int x, int y) {
  int pos = m_doc->documentLayout()->hitTest(QPointF(x, y), Qt::FuzzyHit);
  if (pos == -1) {
    return Position();
  }

  for (QTextBlock block = m_doc->begin(); block != m_doc->end(); block = block.next()) {
    for (QTextBlock::Iterator it = block.begin(); !(it.atEnd()); ++it) {

      QTextFragment frag(it.fragment());

      int start = frag.position();
      int end = start + frag.length();

      if (pos >= start && pos <= end && frag.length() != 1) {
	if (!frag.charFormat().hasProperty(BOOKMARKS_PROPERTY)) {
	  // Only ayat
	  return Position();
	}

	unsigned b = frag.charFormat().property(BOOKMARKS_PROPERTY).value<unsigned>();

	int sura = m_bookmarks->sura(b);
	int aya = m_bookmarks->aya(b);

	return Position(sura, aya);
      }
    }
  }

  return Position();
}
