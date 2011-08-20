#include "index.h"
#include <QSettings>
#include <QFile>
#include <QStringList>

#define LINE_COUNT 6236

bool Index::write(const QString& path, const QList<QPair<off_t, size_t> >& offsets,
		  const QMap<QString, QVariant>& meta) {

  if (offsets.size() != LINE_COUNT) {
    return false;
  }

  QSettings s(path, QSettings::IniFormat);

  s.beginGroup("General");

  for (QMap<QString, QVariant>::const_iterator iter = meta.constBegin();
       iter != meta.constEnd(); iter++) {
    s.setValue(iter.key(), iter.value());
  }

  s.endGroup();

  s.beginWriteArray("offsets");

  for (int x = 0; x < offsets.size(); x++) {
    s.setArrayIndex(x);
    qlonglong off = offsets.at(x).first;
    s.setValue("offset", off);
    s.setValue("len", offsets.at(x).second);
  }

  s.endArray();

  s.sync();

  if (s.status() != QSettings::NoError) {
    QFile(path).remove();
    return false;
  }

  return true;
}

bool Index::read(const QString& path, QList<QPair<off_t, size_t> >& offsets,
		 QMap<QString, QVariant>& meta) {

  QSettings s(path, QSettings::IniFormat);

  s.beginGroup("General");

  QStringList keys = s.childKeys();

  foreach (const QString& key, keys) {
    meta.insert(key, s.value(key));
  }

  s.endGroup();

  int count = s.beginReadArray("offsets");
  if (count != LINE_COUNT) {
    return false;
  }

  for (int x = 0; x < count; x++) {
    s.setArrayIndex(x);
    off_t offset = s.value("offset", -1).toInt();
    size_t len = s.value("len", 0).toInt();

    if (offset == -1 || len == 0) {
      return false;
    }

    offsets << qMakePair<off_t, size_t>(offset, len);
  }

  s.endArray();

  if (s.status() != QSettings::NoError) {
    return false;
  }

  return true;
}
