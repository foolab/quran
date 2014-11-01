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

#ifndef ANDROID_SUPPORT_H
#define ANDROID_SUPPORT_H

#include <QObject>

class AndroidSupport : public QObject {
  Q_OBJECT
  Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged);
  Q_ENUMS(Orientation);

public:
  typedef enum {
    OrientationAll = 0,
    OrientationLandscape = 1,
    OrientationPortrait = 2,
  } Orientation;

  AndroidSupport(QObject *parent = 0);
  ~AndroidSupport();

  Orientation orientation() const;
  void setOrientation(const Orientation& orientation);

public slots:
  void applyOrientation();

signals:
  void orientationChanged();

private:
  Orientation m_orientation;
};

#endif /* ANDROID_SUPPORT_H */
