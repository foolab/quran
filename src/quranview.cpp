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

#include "quranview.h"
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

QuranView::QuranView(QDeclarativeItem *parent) :
  QDeclarativeItem(parent), m_page(0),
  m_data(0), m_bookmarks(0), m_formatter(0),
  m_doc(new QTextDocument(this)) {

  m_doc->setDocumentMargin(0);
  m_doc->setUndoRedoEnabled(false);

  setFlag(ItemHasNoContents, false);
  setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
  //  setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QuranView::~QuranView() {
  m_data = 0;
}

void QuranView::componentComplete() {
  QDeclarativeItem::componentComplete();

  // Do our job

  Q_ASSERT(m_data != 0);

  if (!m_data) {
    return;
  }

  populate();
}

void QuranView::setFont(const QFont& font) {
  m_doc->setDefaultFont(font);

  if (isComponentComplete()) {
    updateLayout();
    update();
  }
}

QFont QuranView::font() {
  return m_doc->defaultFont();
}

void QuranView::setPage(int page) {
  m_page = page;
}

int QuranView::page() {
  return m_page;
}

qreal QuranView::margin() {
  return m_doc->documentMargin();
}

void QuranView::setMargin(qreal margin) {
  m_doc->setDocumentMargin(margin);
}

void QuranView::setDataProvider(DataProvider *data) {
  m_data = data;
}

DataProvider *QuranView::dataProvider() {
  return m_data;
}

void QuranView::setBookmarks(Bookmarks *bookmarks) {
  m_bookmarks = bookmarks;
}

Bookmarks *QuranView::bookmarks() {
  return m_bookmarks;
}

void QuranView::setFormatter(NumberFormatter *formatter) {
  m_formatter = formatter;
}

void QuranView::setHighlightColor(const QColor& color) {
  m_color = color;
}

QColor QuranView::highlightColor() const {
  return m_color;
}

NumberFormatter *QuranView::formatter() {
  return m_formatter;
}

void QuranView::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  if (isComponentComplete() && newGeometry.width() != oldGeometry.width()) {
    updateLayout();
  }

  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
}

void QuranView::updateLayout() {
  m_doc->setTextWidth(width());
  setImplicitHeight(m_doc->size().height());
}

void QuranView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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

void QuranView::populate() {
  Page p = m_data->pageFromIndex(m_page);

  QList<Fragment> frags = p.fragments();

  begin(frags);

  QTextCursor c(m_doc);

  foreach (const Fragment& frag, frags) {
    addFragment(c, frag);
  }

  end(c, frags);
}

void QuranView::begin(const QList<Fragment>& frags) {
  Q_UNUSED(frags);

  m_doc->clear();
}

void QuranView::addFragment(QTextCursor& cursor, const Fragment& frag) {
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

void QuranView::end(QTextCursor& cursor, const QList<Fragment>& frags) {
  if (!frags.isEmpty() && frags.at(0).start() == 0) {
    // Seems we will get an empty line at the beginning
    // of the document if we start it with a block.
    // QTextCursor::movePosition(QTextCursor::Start) before the first insertion
    // doesn't seem to work :|
    cursor.setPosition(0);
    cursor.deleteChar();
  }

  // TODO: account for margin ?
  // TODO: fix this ?
  //  setBaselineOffset(m_doc->size().height());

  m_doc->setTextWidth(width());
  setImplicitHeight(m_doc->size().height());
}

QString QuranView::textForPosition(int x, int y) {
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

QVariant QuranView::bookmarkId(int x, int y) {
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

void QuranView::selectRequested(int sura, int aya) {
  uint bookmark = m_bookmarks->serialize(sura, aya);

  qreal height = 0;

  clearSelection();

  QTextCursor c(m_doc);

  for (QTextBlock block = m_doc->begin(); block != m_doc->end(); block = block.next()) {
    for (QTextBlock::Iterator it = block.begin(); !(it.atEnd()); ++it) {

      QTextFragment frag(it.fragment());

      if (frag.charFormat().hasProperty(BOOKMARKS_PROPERTY) &&
	  frag.charFormat().property(BOOKMARKS_PROPERTY).value<uint>() == bookmark) {

	int start = frag.position();
	int end = start + frag.length();

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

	// calculate line position
        QRectF startRect = block.layout()->lineForTextPosition(start - block.position()).naturalTextRect();
        qreal upper = height + startRect.y();
	upper += m_doc->documentMargin();

        QRectF endRect = block.layout()->lineForTextPosition(end - block.position()).naturalTextRect();
        qreal lower = height + endRect.y() + endRect.height();
	lower += m_doc->documentMargin();
	// done!
	update();

	QMetaObject::invokeMethod(this, "lineVisibilityRequested", Qt::QueuedConnection,
				  Q_ARG(qreal, upper), Q_ARG(qreal, lower));

	return;
      }
    }

    height += m_doc->documentLayout()->blockBoundingRect(block).height();
  }
}

void QuranView::clearSelection() {
  QTextCursor c(m_doc);

  if (m_highlighted.isValid()) {
    QTextCharFormat fmt(m_highlighted.charFormat());
    c.setPosition(m_highlighted.position(), QTextCursor::MoveAnchor);
    c.setPosition(m_highlighted.position() + m_highlighted.length(), QTextCursor::KeepAnchor);

    fmt.clearBackground();
    c.setCharFormat(fmt);
    c.clearSelection();

    update();
  }
}

void QuranView::mouseClicked(int x, int y) {
  int pos = m_doc->documentLayout()->hitTest(QPointF(x, y), Qt::FuzzyHit);
  if (pos == -1) {
    return;
  }

  clearSelection();

  QTextCursor c(m_doc);

  qreal height = 0;
  for (QTextBlock block = m_doc->begin(); block != m_doc->end(); block = block.next()) {
    for (QTextBlock::Iterator it = block.begin(); !(it.atEnd()); ++it) {

      QTextFragment frag(it.fragment());

      int start = frag.position();
      int end = start + frag.length();

      if (pos >= start && pos <= end && frag.length() != 1) {
	if (!frag.charFormat().hasProperty(BOOKMARKS_PROPERTY)) {
	  // Only select ayat
	  return;
	}

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

	// calculate line position
        QRectF startRect = block.layout()->lineForTextPosition(start - block.position()).naturalTextRect();
        qreal upper = height + startRect.y();
	upper += m_doc->documentMargin();

        QRectF endRect = block.layout()->lineForTextPosition(end - block.position()).naturalTextRect();
        qreal lower = height + endRect.y() + endRect.height();
	lower += m_doc->documentMargin();
	// done!
	update();

	QMetaObject::invokeMethod(this, "lineVisibilityRequested", Qt::QueuedConnection,
				  Q_ARG(qreal, upper), Q_ARG(qreal, lower));

	return;
      }
    }

    height += m_doc->documentLayout()->blockBoundingRect(block).height();
  }
}
