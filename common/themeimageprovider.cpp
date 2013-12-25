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

#include "themeimageprovider.h"
#include <QImageReader>
#include <QDebug>
#include <QDir>

ThemeImageProvider::ThemeImageProvider(const QString& path, QObject *parent)
  : QObject(parent), QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap), m_path(path) {

}

ThemeImageProvider::~ThemeImageProvider() {

}

QString ThemeImageProvider::path(const QString& id) const {
  return QString("%1%2%3.png").arg(m_path).arg(QDir::separator()).arg(id);
}

QPixmap ThemeImageProvider::requestPixmap(const QString & id, QSize *size,
					  const QSize& requestedSize) {
  *size = requestedSize;
  QImageReader r(path(id));
  r.setScaledSize(requestedSize);
  QPixmap pix = QPixmap::fromImage(r.read());

  if (!pix.isNull()) {
    return pix;
  }

  r.setFileName(path(QString("blue/%1").arg(id.section('/', 1, 1))));
  r.setScaledSize(requestedSize);
  return QPixmap::fromImage(r.read());
}

QStringList ThemeImageProvider::themes() const {
  return QDir(m_path).entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
}
