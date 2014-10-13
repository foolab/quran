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

#ifndef PHONE_FLIP_CONTROL_H
#define PHONE_FLIP_CONTROL_H

#include <QObject>
#ifndef QT_VERSION_5
#include <qmobilityglobal.h>
#endif

#ifndef QT_VERSION_5
QTM_BEGIN_NAMESPACE
#endif

class QOrientationSensor;
#ifndef QT_VERSION_5
QTM_END_NAMESPACE

QTM_USE_NAMESPACE;
#endif

class PhoneFlipControl : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged);

public:
  PhoneFlipControl(QObject *parent = 0);
  ~PhoneFlipControl();

  bool isActive() const;
  void setActive(bool active);

signals:
  void activeChanged();
  void flipped();

private slots:
  void sensorReadingChanged();

private:
  QOrientationSensor *m_sensor;
};

#endif /* PHONE_FLIP_CONTROL_H */
