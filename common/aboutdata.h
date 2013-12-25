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

#ifndef ABOUT_DATA_H
#define ABOUT_DATA_H

#include <QObject>

class AboutData : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString copyright READ copyright CONSTANT);
  Q_PROPERTY(QString appLicense READ appLicense CONSTANT);
  Q_PROPERTY(QString textLicense READ textLicense CONSTANT);
  Q_PROPERTY(QString transLicense READ transLicense CONSTANT);
  Q_PROPERTY(QString contributors READ contributors CONSTANT);

public:
  AboutData(QObject *parent = 0);
  ~AboutData();

  QString copyright() const;
  QString appLicense() const;
  QString textLicense() const;
  QString transLicense() const;
  QString contributors() const;
  QString platform() const;
};

#endif /* ABOUT_DATA_H */
