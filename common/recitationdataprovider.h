/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef RECITATION_DATA_PROVIDER_H
#define RECITATION_DATA_PROVIDER_H

#include <QByteArray>
#include <QString>

class QuaZip;
class Media;

class RecitationDataProvider {
public:
  RecitationDataProvider(const QString& path);
  ~RecitationDataProvider();

  QByteArray data(const Media& media);
  bool setData(const Media& media, const QByteArray& data);

  bool isLocal() const;

private:
  QString localPath(const Media& media);

  QString m_path;
  QString m_sub;
  QuaZip *m_zip;
};

#endif /* RECITATION_DATA_PROVIDER_H */
