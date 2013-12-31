/*
 * Copyright (c) 2012-2013 Mohammed Sameer <msameer@foolab.org>.
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
#ifndef SAILFISH
#include <qmobilityglobal.h>
#endif

class Settings;

#ifndef SAILFISH
QTM_BEGIN_NAMESPACE
#endif

class QOrientationSensor;
#ifndef SAILFISH
QTM_END_NAMESPACE

QTM_USE_NAMESPACE;
#endif

class PhoneFlipControl : public QObject {
  Q_OBJECT

public:
  PhoneFlipControl(Settings *settings, QObject *parent = 0);
  ~PhoneFlipControl();

  void start();
  void stop();

signals:
  void flipped();

private slots:
  void flipToStopRecitationChanged();
  void sensorReadingChanged();

private:
  Settings *m_settings;

  QOrientationSensor *m_sensor;

  bool m_run;
};

#endif /* PHONE_FLIP_CONTROL_H */
