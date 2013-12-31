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

#ifndef THEME_IMAGE_PROVIDER_H
#define THEME_IMAGE_PROVIDER_H

#ifdef SAILFISH
#include <QQuickImageProvider>
#define PARENT QQuickImageProvider
#else
#include <QDeclarativeImageProvider>
#define PARENT QDeclarativeImageProvider
#endif
#include <QObject>

class ThemeImageProvider : public QObject, public PARENT {
  Q_OBJECT

public:
  ThemeImageProvider(const QString& path, QObject *parent = 0);
  ~ThemeImageProvider();

  virtual QPixmap requestPixmap(const QString & id, QSize *size, const QSize& requestedSize);

  Q_INVOKABLE QString path(const QString& id) const;
  Q_INVOKABLE QStringList themes() const;

private:
  const QString m_path;
};

#endif /* THEME_IMAGE_PROVIDER_H */
