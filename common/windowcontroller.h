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
#include <QRectF>

class WindowController : public QObject {
  Q_OBJECT
  Q_PROPERTY(int width READ width NOTIFY widthChanged);
  Q_PROPERTY(int height READ height NOTIFY heightChanged);
  Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged);
  Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged);

public:
  WindowController(QObject *parent = 0);
  ~WindowController();

  int width() const;
  int height() const;
  int orientation() const;
  void setOrientation(int orientation);
  bool fullScreen() const;
  void setFullScreen(bool fullScreen);

signals:
  void fullScreenChanged();
  void widthChanged();
  void heightChanged();
  void orientationChanged();

public slots:
  void show();
  void exposedContentRectChanged();
  void applyOrientation();

private:
  QRectF m_rect;
  bool m_fullScreen;
  int m_orientation;
};

#endif /* WINDOW_CONTROLLER_H */
