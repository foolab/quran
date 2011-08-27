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

#ifndef THEME_IMAGE_PROVIDER_H
#define THEME_IMAGE_PROVIDER_H

#include <QDeclarativeImageProvider>
#include <QObject>

class ThemeImageProvider : public QObject, public QDeclarativeImageProvider {
  Q_OBJECT

  Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged);

public:
  ThemeImageProvider(const QString& path, QObject *parent = 0);
  ~ThemeImageProvider();

  void setId(const QString& id);
  QString id() const;

  virtual QPixmap requestPixmap(const QString & id, QSize *size, const QSize& requestedSize);

  Q_INVOKABLE QString path(const QString& id) const;

signals:
  void idChanged();

private:
  const QString m_path;
  QString m_id;
};

#endif /* THEME_IMAGE_PROVIDER_H */
