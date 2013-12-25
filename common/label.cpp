/*
 * Copyright (c) 2011-2013 Mohammed Sameer <msameer@foolab.org>.
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

#include "label.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QDebug>

Label::Label(QDeclarativeItem *parent)
  : QDeclarativeItem(parent), m_doc(new QTextDocument(this)) {

  setFont(QFont("Nokia Pure Text", 24));

  setColor(Qt::black);

  m_doc->setDocumentMargin(0);
  m_doc->setUndoRedoEnabled(false);

  setFlag(ItemHasNoContents, false);
  setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
}

Label::~Label() {

}

void Label::setFont(const QFont& font) {
  if (Label::font() != font) {
    m_doc->setDefaultFont(font);

    if (isComponentComplete()) {
      updateLayout();
      update();
    }
  }
}

QFont Label::font() const {
  return m_doc->defaultFont();
}

void Label::setColor(const QColor& color) {
  if (m_color != color) {
    m_color = color;

    if (isComponentComplete()) {
      update();
    }
  }
}

QColor Label::color() const {
  return m_color;
}

void Label::setText(const QString& text) {
  if (Label::text() != text) {
    m_doc->setPlainText(text);

    if (isComponentComplete()) {
      updateLayout();
      update();
    }
  }
}

QString Label::text() const {
  return m_doc->toPlainText();
}

void Label::setCenter(bool center) {
  if (Label::center() != center) {
    QTextOption opt = m_doc->defaultTextOption();
    Qt::Alignment a = opt.alignment();

    if (center) {
      a = Qt::AlignCenter;
    }
    else {
      a = 0;
    }

    opt.setAlignment(a);

    m_doc->setDefaultTextOption(opt);

    if (isComponentComplete()) {
      updateLayout();
      update();
    }
  }
}

bool Label::center() const {
  return m_doc->defaultTextOption().alignment().testFlag(Qt::AlignCenter);
}

void Label::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		  QWidget *widget) {

  Q_UNUSED(widget);

  QAbstractTextDocumentLayout::PaintContext ctx;

  if (option->exposedRect.isValid()) {
    qreal margin = m_doc->documentMargin();
    ctx.clip = option->exposedRect.adjusted(margin, margin, -margin, -margin);
  }

  ctx.palette.setColor(QPalette::Text, m_color);

  m_doc->documentLayout()->draw(painter, ctx);
}

void Label::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

  if (isComponentComplete() && newGeometry.width() != oldGeometry.width()) {
    updateLayout();
    update();
  }
}

void Label::updateLayout() {
  prepareGeometryChange();

  m_doc->setTextWidth(width());

  setImplicitHeight(m_doc->size().height());
}
