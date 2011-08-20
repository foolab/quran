#ifndef INDEX_H
#define INDEX_H

#include <QList>
#include <QPair>
#include <QString>
#include <QMap>
#include <QVariant>

class Index {
public:
  static bool write(const QString& path, const QList<QPair<off_t, size_t> >& offsets,
		    const QMap<QString, QVariant>& meta);

  static bool read(const QString& path, QList<QPair<off_t, size_t> >& offsets,
		   QMap<QString, QVariant>& meta);
};

#endif /* INDEX_H */
