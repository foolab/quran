/*!
 * Copyright (C) 2011-2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "toolbarlayout.h"

ToolBarLayout::ToolBarLayout(PARENT *parent) :
  PARENT(parent) {

#ifndef SAILFISH
  setFlag(ItemHasNoContents, false);
#endif

  QObject::connect(this, SIGNAL(visibleChanged()), this, SLOT(itemVisibilityChanged()));
}

ToolBarLayout::~ToolBarLayout() {

}

void ToolBarLayout::componentComplete() {
  PARENT::componentComplete();

  if (isVisible()) {
    reposition();
  }
}

void ToolBarLayout::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  PARENT::geometryChanged(newGeometry, oldGeometry);

  if (isComponentComplete() && isVisible()) {
    reposition();
  }
}

void ToolBarLayout::itemVisibilityChanged() {
  if (isComponentComplete() && isVisible()) {
    reposition();
  }
}

void ToolBarLayout::reposition() {
  QList<PARENT *> items;
  qreal totalWidth = 0;

  foreach (PARENT *v, m_items) {
    if (v->isVisible()) {
      items << v;
      totalWidth += v->width();
    }
  }

  if (items.size() < 1) {
    return;
  }

  qreal spacing = (width() - totalWidth) / (items.size() - 1);

  qreal height = PARENT::height();

  totalWidth = 0;
  for (int x = 0; x < items.size(); x++) {
    PARENT *item = items[x];
    item->setY((height - item->height()) / 2);
    item->setX((x * spacing) + totalWidth);

    totalWidth += item->width();
  }
}

void ToolBarLayout::clearItems() {
  foreach (PARENT *v, m_items) {
    QObject::disconnect(v, SIGNAL(visibleChanged()), this, SLOT(reposition()));
  }

  m_items.clear();
}

void ToolBarLayout::setItems(const QVariant& children) {
  clearItems();

#ifdef SAILFISH
  QQmlListReference items = children.value<QQmlListReference>();
#else
  QDeclarativeListReference items = children.value<QDeclarativeListReference>();
#endif

  for (int x = 0; x < items.count(); x++) {
    QObject *obj = items.at(x);

    if (PARENT *v = qobject_cast<PARENT *>(obj)) {
      m_items << v;
      QObject::connect(v, SIGNAL(visibleChanged()), this, SLOT(reposition()));
    }
  }

  if (isComponentComplete() && isVisible()) {
    reposition();
  }
}
