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

#ifndef RECITATION_H
#define RECITATION_H

#include <QString>
#include <QUrl>

class Recitation {
public:
  static Recitation *create(const QString& id, const QString& dir);

  virtual ~Recitation();

  QString id() const;
  QString name() const;
  QString dir() const;

  bool isValid();

  virtual QUrl mediaUrl(int chapter, int verse) = 0;

protected:
  Recitation(const QString& name, const QString& id, const QString& dir);

private:
  const QString m_name;
  const QString m_id;
  const QString m_dir;
};

#endif /* RECITATION_H */
