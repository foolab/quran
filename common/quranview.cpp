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
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include "bookmarks.h"
#include "dataprovider.h"
#include "numberformatter.h"

QuranView::QuranView(QDeclarativeItem *parent)
  : QDeclarativeItem(parent), AbstractQuranView(new QTextDocument(this)),
    m_page(-1) {

  m_doc->setDocumentMargin(0);
  m_doc->setUndoRedoEnabled(false);

  setFlag(ItemHasNoContents, false);
  setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
  //  setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QuranView::~QuranView() {

}

void QuranView::updateLayout() {
  prepareGeometryChange();

  m_doc->setTextWidth(width());

  setImplicitHeight(m_doc->size().height());
}

void QuranView::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  if (isComponentComplete() && newGeometry.width() != oldGeometry.width()) {
    updateLayout();
  }

  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
}

void QuranView::setPage(int page) {
  m_page = page;

  if (m_page != -1 && isComponentComplete()) {
    populate();
  }
}

int QuranView::page() {
  return m_page;
}

void QuranView::setMargin(qreal margin) {
  m_doc->setDocumentMargin(margin);

  if (isComponentComplete()) {
    updateLayout();
    update();
  }
}

qreal QuranView::margin() {
  return m_doc->documentMargin();
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

void QuranView::populate() {
  AbstractQuranView::populate(m_page);

  updateLayout();
}

void QuranView::componentComplete() {
  QDeclarativeItem::componentComplete();

  if (m_page != -1) {
    populate();
  }
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

void QuranView::mouseClicked(int x, int y) {
  Position pos = position(x, y);
  QLineF line = position(pos);

  if (!line.isNull()) {
    emit lineVisibilityRequested(line.y1(), line.y2());
  }
}

void QuranView::scrollTo(int sura, int aya) {
  Position pos(sura, aya);
  QLineF line = position(pos);

  if (!line.isNull()) {
    emit lineVisibilityRequested(line.y1(), line.y2());
  }
}

QVariant QuranView::bookmarkId(int x, int y) {
  Position pos = position(x, y);

  if (!pos.isValid()) {
    return QVariant();
  }

  return m_bookmarks->serialize(pos.sura(), pos.aya());
}

QString QuranView::textForPosition(int x, int y) {
  Position pos = position(x, y);
  if (!pos.isValid()) {
    return QString();
  }

  return QString("%1 (%2)").arg(m_data->text(pos.sura(), pos.aya()))
    .arg(m_formatter->number(pos.aya() + 1));
}
