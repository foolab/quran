// -*- c++ -*-

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

#ifndef TOOL_BAR_LAYOUT_H
#define TOOL_BAR_LAYOUT_H

#ifdef PARENT
#undef PARENT
#endif

#ifdef SAILFISH
#include <QQuickItem>
#define PARENT QQuickItem
#else
#include <QDeclarativeItem>
#define PARENT QDeclarativeItem
#endif

class ToolBarLayout : public PARENT {
  Q_OBJECT

public:
  ToolBarLayout(PARENT *parent = 0);
  ~ToolBarLayout();

  void componentComplete();

public slots:
  void clearItems();
  void setItems(const QVariant& children);

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

private slots:
  void reposition();
  void itemVisibilityChanged();

private:
  QList<PARENT *> m_items;
};

#endif /* TOOL_BAR_LAYOUT_H */
