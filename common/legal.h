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

#ifndef LEGAL_H
#define LEGAL_H

#include <QObject>

class Legal : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString copyright READ copyright CONSTANT);
  Q_PROPERTY(QString appLicense READ appLicense CONSTANT);
  Q_PROPERTY(QString textLicense READ textLicense CONSTANT);
  Q_PROPERTY(QString fontLicense READ fontLicense CONSTANT);

public:
  Legal(QObject *parent = 0);
  ~Legal();

  QString copyright();
  QString appLicense();
  QString textLicense();
  QString fontLicense();
};

#endif /* LEGAL_H */
