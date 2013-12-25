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

#ifndef WINDOW_CONTROLLER_H
#define WINDOW_CONTROLLER_H

#include <QObject>

class QWidget;
class Settings;
class QDeclarativeItem;

class WindowController : public QObject {
  Q_OBJECT

public:
  WindowController(QWidget *view, Settings *settings, QDeclarativeItem *root, QObject *parent = 0);
  ~WindowController();

public slots:
  void show();

  void setOrientation();

  void exposedContentRectChanged();

private:
  QWidget *m_view;
  Settings *m_settings;
  QDeclarativeItem *m_root;
};

#endif /* WINDOW_CONTROLLER_H */
