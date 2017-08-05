/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
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

#include "iconprovider.h"
#include <QDebug>
#include <QPixmap>
#include <QPainter>

/*
 * Code adapted from here:
 * https://together.jolla.com/question/44325/iconbutton-how-to-use-own-icons-with-highlight/
 */
IconProvider::IconProvider() :
  QQuickImageProvider(QQuickImageProvider::Pixmap) {

}

IconProvider::~IconProvider() {

}

QPixmap IconProvider::requestPixmap(const QString& id, QSize *size, const QSize& requestedSize) {
  QStringList parts = id.split('?');

  if (parts.size() <= 0) {
    qWarning() << "Failed to parse" << id;
    return QPixmap();
  }

  QPixmap sourcePixmap = QPixmap::fromImage(QImage(":/icons/"+parts[0]));

  if (size) {
    *size  = sourcePixmap.size();
  }


  if ((parts.length() > 1) && QColor::isValidColor(parts[1])) {
    QPainter painter(&sourcePixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(sourcePixmap.rect(), parts[1]);
    painter.end();
  }

  if (requestedSize.width() > 0 && requestedSize.height() > 0) {
    return sourcePixmap.scaled(requestedSize.width(),
			       requestedSize.height(), Qt::IgnoreAspectRatio);
  } else {
    return sourcePixmap;
  }
}
