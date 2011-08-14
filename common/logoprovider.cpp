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

#include "logoprovider.h"
#include <QImageReader>

LogoProvider::LogoProvider() : QDeclarativeImageProvider(QDeclarativeImageProvider::Image) {

}

LogoProvider::~LogoProvider() {

}

QImage LogoProvider::requestImage(const QString& id, QSize *size, const QSize& requestedSize) {
  Q_UNUSED(id);

  QImageReader reader(DATA_DIR "/logo_160.png");
  reader.setScaledSize(requestedSize);

  *size = requestedSize;

  return reader.read();
}
