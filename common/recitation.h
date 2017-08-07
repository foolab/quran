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

#ifndef RECITATION_H
#define RECITATION_H

#include <QObject>
#include <QString>
#include <QUrl>

class Media;
class RecitationInfo;
class QIODevice;
class QuaZip;

class Recitation : public QObject {
  Q_OBJECT

  Q_ENUMS(Type Status);

  Q_PROPERTY(QString name READ name CONSTANT);
  Q_PROPERTY(QString uuid READ uuid CONSTANT);
  Q_PROPERTY(QString quality READ quality CONSTANT);
  Q_PROPERTY(Type type READ type CONSTANT);
  Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged);
  Q_PROPERTY(bool loaded READ isLoaded NOTIFY loadedChanged);

public:
  enum Type {
    Unzipped = 0,
    Zipped,
    Simple,
    Online,
  };

  enum Status {
    None,
    Installed
  };

  static RecitationInfo *guessType(const QString& recitationDir);

  Recitation(RecitationInfo *info, QObject *parent = 0);
  ~Recitation();

  QUrl playBackUrl(const Media& media);
  QUrl downloadUrl(const Media& media);
  QByteArray data(const Media& media);
  bool setData(const Media& media, const QByteArray& data);

  bool isLoaded() const;
  void setLoaded(bool loaded);

  Q_INVOKABLE bool enable();
  Q_INVOKABLE bool disable();

  /*
    Q_INVOKABLE bool clear();
    Q_INVOKABLE bool download();
    Q_INVOKABLE bool remove();
  */

  QString uuid() const;
  QString name() const;
  QString quality() const;

  Type type() const;
  Status status() const;
  void setStatus(Status status);

signals:
  void statusChanged();
  void enabled();
  void disabled();
  void loadedChanged();

private:
  static RecitationInfo *parseZekr(QIODevice *dev, const Type& type);

  QString path(const QString fileName) const;

  RecitationInfo *m_info;
  bool m_loaded;
  QuaZip *m_zip;
};

class RecitationInfo {
public:
  QString m_uuid;
  QString m_name;
  QString m_quality;
  QString m_dir;
  QString m_subdir;
  QUrl m_url;
  Recitation::Type m_type;
  Recitation::Status m_status;
};

#endif /* RECITATION_H */
