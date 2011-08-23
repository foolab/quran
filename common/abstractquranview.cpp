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
#include <QTextList>

#define BOOKMARKS_PROPERTY QTextFormat::UserProperty

AbstractQuranView::AbstractQuranView(QTextDocument *doc) :
  m_doc(doc), m_data(0), m_bookmarks(0), m_formatter(0),
  m_addSecondaryText(false) {

  m_doc->setDocumentMargin(0);
  m_doc->setUndoRedoEnabled(false);
  m_doc->setIndentWidth(0);
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
  m_highlightColor = color;
}

QColor AbstractQuranView::highlightColor() const {
  return m_highlightColor;
}

void AbstractQuranView::setTitleColor(const QColor& color) {
  m_titleColor = color;
}

QColor AbstractQuranView::titleColor() const {
  return m_titleColor;
}

void AbstractQuranView::setSubtitleColor(const QColor& color) {
  m_subtitleColor = color;
}

QColor AbstractQuranView::subtitleColor() const {
  return m_subtitleColor;
}

void AbstractQuranView::setVerseColor(const QColor& color) {
  m_verseColor = color;
}

QColor AbstractQuranView::verseColor() const {
  return m_verseColor;
}

void AbstractQuranView::setAddSecondaryText(bool add) {
  m_addSecondaryText = add;
}

bool AbstractQuranView::isSecondaryTextAdded() const {
  return m_addSecondaryText;
}

QTextCharFormat AbstractQuranView::titleCharFormat() {
  QTextCharFormat f;

  f.setForeground(QBrush(m_titleColor));

  return f;
}

QTextCharFormat AbstractQuranView::subtitleCharFormat() {
  QTextCharFormat f;

  f.setForeground(QBrush(m_subtitleColor));

  return f;
}

QTextCharFormat AbstractQuranView::verseCharFormat(int sura, int aya) {
  QTextCharFormat f;

  f.setProperty(BOOKMARKS_PROPERTY, m_bookmarks->serialize(sura, aya));

  f.setForeground(QBrush(m_verseColor));

  return f;
}

QTextCharFormat AbstractQuranView::verseCharFormat(const Position& pos) {
  return verseCharFormat(pos.sura(), pos.aya());
}

QTextCharFormat AbstractQuranView::secondaryCharFormat(int sura, int aya) {
  QTextCharFormat f(verseCharFormat(sura, aya));

  f.setProperty(QTextFormat::FontFamily, "Nokia Sana"); // TODO:

  return f;
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

void AbstractQuranView::begin(const QList<Fragment>& frags) {
  Q_UNUSED(frags);

  m_doc->clear();
}

void AbstractQuranView::addFragment(QTextCursor& cursor, const Fragment& frag) {
  bool addTranslation = m_addSecondaryText && m_data->secondaryTextProvider() != 0;
  bool splitIntoLines = m_data->secondaryTextProvider() != 0;

  Sura s = m_data->sura(frag.sura());

  int sura = frag.sura();

  const QStringList text = m_data->text(frag);
  const QStringList secondary = m_data->secondaryText(frag);

  if (frag.start() == 0) {
    insertBlock(QStringList() << m_data->fullSuraName(frag.sura()), cursor,
		QList<QTextCharFormat>() << titleCharFormat(), true);

    if (s.hasBasmala()) {
      insertBlock(QStringList() << m_data->basmala(), cursor,
		  QList<QTextCharFormat>() << subtitleCharFormat(), true);
    }
  }

  if (splitIntoLines || (addTranslation && secondary.size() == text.size())) {
    for (int x = 0; x < text.size(); x++) {
      int ayaNumber = x + frag.start();

      QString verse = QString("%1 (%2)").arg(text.at(x)).arg(m_formatter->number(ayaNumber + 1));

      QTextList *list = cursor.insertList(QTextListFormat());
      QTextBlockFormat bf = cursor.blockFormat();
      bf.clearProperty(QTextFormat::BlockAlignment);

      cursor.setBlockFormat(bf);
      cursor.insertText(verse, verseCharFormat(sura, ayaNumber));

      if (addTranslation) {
	cursor.insertBlock(QTextBlockFormat());
	cursor.insertText(secondary.at(x), secondaryCharFormat(sura, ayaNumber));
	list->add(cursor.block());
      }
    }
  }
  else {
    QTextCharFormat f;
    f.setForeground(QBrush(m_verseColor));

    QStringList verses;
    QList<QTextCharFormat> fmt;

    for (int x = 0; x < text.size(); x++) {
      int ayaNumber = x + frag.start();

      verses << QString("%1 (%2) ").arg(text.at(x)).arg(m_formatter->number(ayaNumber + 1));
      f.setProperty(BOOKMARKS_PROPERTY, m_bookmarks->serialize(sura, ayaNumber));
      fmt << f;
    }

    insertBlock(verses, cursor, fmt, false);
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

void AbstractQuranView::insertBlock(const QStringList& text, QTextCursor& cursor,
				    const QList<QTextCharFormat>& fmt, bool center) {
  QTextBlockFormat bfmt;
  if (center) {
    bfmt.setAlignment(Qt::AlignHCenter);
  }

  cursor.insertBlock(bfmt);

  for (int x = 0; x < text.size() - 1; x++) {
    cursor.insertText(text.at(x), fmt.at(x));
  }

  QString last = text.last();
  if (last.endsWith(" ")) {
    last.chop(1);
  }

  cursor.insertText(last, fmt.last());
}

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
	  fmt.setBackground(m_highlightColor);
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

QTextBlock AbstractQuranView::blockAtPosition(int x, int y) {
  int pos = m_doc->documentLayout()->hitTest(QPointF(x, y), Qt::FuzzyHit);
  if (pos == -1) {
    return QTextBlock();
  }

  for (QTextBlock block = m_doc->begin(); block != m_doc->end(); block = block.next()) {
    for (QTextBlock::Iterator it = block.begin(); !(it.atEnd()); ++it) {

      QTextFragment frag(it.fragment());

      int start = frag.position();
      int end = start + frag.length();

      if (pos >= start && pos <= end && frag.length() != 1) {
	if (!frag.charFormat().hasProperty(BOOKMARKS_PROPERTY)) {
	  // Only ayat
	  return QTextBlock();
	}

	return block;
      }
    }
  }

  return QTextBlock();
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

bool AbstractQuranView::toggleSecondaryText(int x, int y) {
  QTextBlock b = blockAtPosition(x, y);
  if (!b.isValid()) {
    return false;
  }

  QTextList *list = b.textList();
  if (!list) {
    return false;
  }

  if (list->count() == 2) {
    QTextCursor c(m_doc);
    QTextBlock b = list->item(1);
    list->remove(b);
    c.setPosition(b.position());
    c.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    c.deleteChar();
    c.deleteChar();
  }
  else if (list->count() == 1) {
    Position pos = position(x, y);
    QTextCursor c(m_doc);

    c.setPosition(b.position());
    c.movePosition(QTextCursor::EndOfBlock);
    c.insertBlock();
    c.insertText(m_data->secondaryText(pos.sura(), pos.aya()),
		 secondaryCharFormat(pos.sura(), pos.aya()));
    list->add(c.block());
  }

  return true;
}
