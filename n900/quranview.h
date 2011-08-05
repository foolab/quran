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

#include <QTextEdit>
#include "abstractquranview.h"

class QAbstractKineticScroller;
class Settings;

class QuranView : public QTextEdit, public AbstractQuranView {
  Q_OBJECT

public:
  QuranView(Settings *settings, QWidget *parent = 0);
  ~QuranView();

  void setY(int y);

  void scrollToPosition(const Position& pos);

protected:
  virtual void mousePressEvent(QMouseEvent *e);
  virtual void mouseReleaseEvent(QMouseEvent *e);
  virtual void contextMenuEvent(QContextMenuEvent *event);

private:
  void showLine(const QLineF& line, const QMatrix& m);
  QMatrix matrix();

  Settings *m_settings;
  QAbstractKineticScroller *m_scroll;
};

#endif /* QURAN_VIEW_H */
