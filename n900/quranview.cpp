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
#include <QMouseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QAbstractKineticScroller>
#include "settings.h"
#include <QMenu>
#include "bookmarks.h"
#include <QWidgetAction>
#include <QLabel>
#include "dataprovider.h"
#include "numberformatter.h"

QuranView::QuranView(Settings *settings, QWidget *parent)
  : QTextEdit(parent), AbstractQuranView(document()), m_settings(settings) {

  setReadOnly(true);
  setTextInteractionFlags(Qt::NoTextInteraction);

  setContextMenuPolicy(Qt::DefaultContextMenu);

  m_scroll = property("kineticScroller").value<QAbstractKineticScroller *>();
  m_scroll->setEnabled(true);
}

QuranView::~QuranView() {

}

void QuranView::setY(int y) {
  m_scroll->scrollTo(QPoint(0, y));
}

void QuranView::showLine(const QLineF& line, const QMatrix& m) {
  if (line.isNull()) {
    return;
  }

  qreal upper = line.y1();
  qreal lower = line.y2();
  QRect r = m.mapRect(QRect(0, 0, width(), viewport()->height()));

  bool containsUpper = r.contains(QPoint(0, upper));
  bool containsLower = r.contains(QPoint(0, lower));

  if (containsUpper && containsLower) {
    // Nothing.
  }
  else if (containsLower) {
    // Topmost part is not visible.
    m_scroll->scrollTo(QPoint(0, upper));
  }
  else {
    // Bottom part is not visible
    if (line.dy() <= viewport()->height()) {
      int diff = viewport()->height() - line.dy();
      // It will fit. Let's scroll the minimum distance needed.
      m_scroll->scrollTo(QPoint(0, upper - diff));
    }
    else {
      // It won't fit no matter what.
      m_scroll->scrollTo(QPoint(0, upper));
    }
  }
}

QMatrix QuranView::matrix() {
  QMatrix m;

  QScrollBar *h = horizontalScrollBar();
  QScrollBar *v = verticalScrollBar();

  m.translate(isRightToLeft() ? h->maximum() - h->value() : h->value(), v->value());

  return m;
}

void QuranView::scrollToPosition(const Position& pos) {
  if (pos.isValid()) {
    QLineF line = position(pos);
    showLine(line, matrix());
  }
}

void QuranView::mousePressEvent(QMouseEvent *e) {
  Q_UNUSED(e);
}

void QuranView::mouseReleaseEvent(QMouseEvent *e) {
  if (e->button() != Qt::LeftButton) {
    e->ignore();
    return;
  }

  e->accept();

  QMatrix m = matrix();

  QPoint p = m.map(e->pos());

  Position pos = position(p.x(), p.y());

  if (pos.isValid()) {
    showLine(position(pos), m);
  }
}

void QuranView::contextMenuEvent(QContextMenuEvent *event) {
  event->accept();

  QMatrix m = matrix();

  QPoint p = m.map(event->pos());

  Position pos = position(p.x(), p.y());

  if (!pos.isValid()) {
    return;
  }

  Bookmarks *b = bookmarks();

  QString aya = QString("<div dir=\"rtl\">%1 (%2)</div>")
    .arg(dataProvider()->text(pos.sura(), pos.aya()))
    .arg(formatter()->number(pos.aya() + 1));
  QLabel *label = new QLabel(aya, this);
  label->setWordWrap(true);
  QFont f = label->font();
  f.setFamily(m_settings->fontFamily());
  label->setFont(f);

  QWidgetAction *a = new QWidgetAction(this);
  a->setDefaultWidget(label);

  QMenu menu;
  menu.addAction(a);

  QAction *add = 0;
  QAction *del = 0;

  if (b->isBookmarked(pos.sura(), pos.aya())) {
    del = menu.addAction(tr("Remove from favorites"));
  }
  else {
    add = menu.addAction(tr("Add to favorites"));
  }

  QAction *res = menu.exec(event->pos());

  if (res == add) {
    b->add(b->serialize(pos.sura(), pos.aya()));
  }
  else if (res == del) {
    b->remove(b->serialize(pos.sura(), pos.aya()));
  }

  //showLine(position(pos), m);
}
