/*
 * Copyright (c) 2020 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef PARCEL_H
#define PARCEL_H

#include <QAndroidParcel>
#include <QAndroidBinder>
#include "bundle.h"

class Parcel : public QAndroidParcel {
public:
  using QAndroidParcel::QAndroidParcel;

  Parcel(const QAndroidParcel& parcel);

  Bundle readBundle();
  void writeBundle(Bundle& bundle);
};

#endif /* PARCEL_H */
