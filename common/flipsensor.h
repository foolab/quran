/*
 * Copyright (c) 2011-2020 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef FLIP_SENSOR_H
#define FLIP_SENSOR_H

#include <QObject>

class QOrientationSensor;

class FlipSensor : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);

public:
  FlipSensor(QObject *parent = 0);
  ~FlipSensor();

  bool isActive() const;
  void setActive(bool active);

signals:
  void activeChanged();
  void flipped();

private:
  QOrientationSensor *m_sensor;
};

#endif /* FLIP_SENSOR_H */
