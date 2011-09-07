#include "recitation.h"
#include <QFile>
#include <QDir>
#include <QSettings>

#define SIMPLE_INFO_FILE               "info.ini"
#define ZEKR_INFO_FILE                 "recitation.properties"

Recitation::~Recitation() {

}

QString Recitation::id() const {
  return m_id;
}

QString Recitation::name() const {
  return m_name;
}

QString Recitation::dir() const {
  return m_dir;
}

bool Recitation::isValid() {
  return !m_id.isEmpty() && !m_name.isEmpty() && !m_dir.isEmpty();
}

Recitation::Recitation(const QString& name, const QString& id, const QString& dir)
  : m_name(name), m_id(id), m_dir(dir) {

}

class RecitationSimple : public Recitation {
public:
  static RecitationSimple *create(const QString& id, const QString& dir) {
    QDir d(dir);
    QString path = d.filePath(SIMPLE_INFO_FILE);

    QFileInfo inf(path);

    if (!inf.exists() || !inf.isReadable()) {
      return 0;
    }

    QString name = QSettings(path, QSettings::IniFormat).value("recitation/name").toString();
    if (name.isEmpty()) {
      return 0;
    }

    return new RecitationSimple(name, id, dir);
  }

  QUrl mediaUrl(int chapter, int verse) {
    QString mp3 = QString("%1/%2%3.mp3").arg(dir()).arg(chapter, 3, 10, QChar('0')).arg(verse, 3, 10, QChar('0'));

    return QUrl::fromLocalFile(mp3);
  }

protected:
  RecitationSimple(const QString& name, const QString& id, const QString& dir) :
    Recitation(name, id, dir) {

  }
};

class RecitationZekr : public Recitation {
public:

  static RecitationZekr *create(const QString& id, const QString& dir) {

    QDir d(dir);
    QString path = d.filePath(ZEKR_INFO_FILE);

    QFileInfo inf(path);

    if (!inf.exists() || !inf.isReadable()) {
      return 0;
    }

    // Look for:
    // audio.id => mp3 subdirectory
    // audio.language => use that to extract a localized audio.reciter
    // audio.reciter => fallback name

    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
      return 0;
    }

    QString subdir, name, localizedName;
    QByteArray lang;

    QMap<QByteArray, QByteArray> map;

    while (!file.atEnd()) {
      QByteArray a = file.readLine();
      a.chop(1); // Remove terminating \n
      QList<QByteArray> as = a.split('=');
      if (as.size() != 2) {
	continue;
      }

      QByteArray key = as[0].trimmed();
      QByteArray val = as[1].trimmed();
      map.insert(key, val);
    }

    if (map.contains("audio.id")) {
      subdir = QString::fromUtf8(map["audio.id"]);
    }

    if (map.contains("audio.language")) {
      lang = map["audio.language"];
    }

    if (map.contains("audio.reciter")) {
      name = QString::fromUtf8(map["audio.reciter"]);
    }

    if (map.contains("audio.reciter." + lang)) {
      localizedName = QString::fromUtf8(map["audio.reciter." + lang]);
    }


    if (name.isEmpty() || subdir.isEmpty()) {
      return 0;
    }

    if (name.isEmpty() && localizedName.isEmpty()) {
      return 0;
    }

    return new RecitationZekr(localizedName.isEmpty() ? name : localizedName,
			      id, QDir(dir).filePath(subdir));
  }

  QUrl mediaUrl(int chapter, int verse) {
    QString mp3 = QString("%1/%2/%2%3.mp3").arg(dir()).arg(chapter, 3, 10, QChar('0')).arg(verse, 3, 10, QChar('0'));

    return QUrl::fromLocalFile(mp3);
  }

private:
  RecitationZekr(const QString& name, const QString& id, const QString& dir)
    : Recitation(name, id, dir) {
  }
};

Recitation *Recitation::create(const QString& id, const QString& dir) {
  Recitation *r = RecitationSimple::create(id, dir);

  if (!r) {
    r = RecitationZekr::create(id, dir);
  }

  if (!r) {
    return 0;
  }

  if (r->isValid()) {
    return r;
  }

  delete r;
  return 0;
}