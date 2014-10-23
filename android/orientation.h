/*
 * Copyright (c) 2011-2014 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <QObject>

class Orientation : public QObject {
  Q_OBJECT
  Q_PROPERTY(OrientationDirection orientation
	     READ orientation
	     WRITE setOrientation
	     NOTIFY orientationChanged);
  Q_ENUMS(OrientationDirection);

public:
  typedef enum {
    OrientationAll = -1,
    OrientationLandscape = 0,
    OrientationPortrait = 1,
  } OrientationDirection;

  Orientation(QObject *parent = 0);
  ~Orientation();

  OrientationDirection orientation() const;
  void setOrientation(const OrientationDirection& orientation);

public slots:
  void applyOrientation();

signals:
  void orientationChanged();

private:
  OrientationDirection m_orientation;
};

#endif /* ORIENTATION_H */
