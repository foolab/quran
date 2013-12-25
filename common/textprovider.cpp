#include "textprovider.h"
#include <QSettings>
#include "index.h"
#include <QFile>
#include <QDebug>

class TextProviderPrivate {
public:
  QList<QPair<off_t, size_t> > offsets;
  QString index;
  QString data;
  char *ptr;
  off_t size;
  int id;
  QFile *file;
};

TextProvider::TextProvider(int id, const QString& dataFile, const QString& indexFile)
  : d_ptr(new TextProviderPrivate) {
  d_ptr->size = 0;
  d_ptr->ptr = 0;
  d_ptr->index = indexFile;
  d_ptr->data = dataFile;
  d_ptr->id = id;
  d_ptr->file = new QFile(dataFile);
}

TextProvider::~TextProvider() {
  unload();

  delete d_ptr->file;
  delete d_ptr;
}

int TextProvider::id() const {
  return d_ptr->id;
}

bool TextProvider::load() {
  if (!d_ptr->offsets.isEmpty()) {
    return true;
  }

  QMap<QString, QVariant> meta;

  if (!Index::read(d_ptr->index, d_ptr->offsets, meta) || !meta.contains("size")) {
    d_ptr->offsets.clear();
    return false;
  }

  qlonglong size = meta["size"].toLongLong();

  if (!d_ptr->file->open(QFile::ReadOnly)) {
    d_ptr->offsets.clear();
    return false;
  }

  if (d_ptr->file->size() != size) {
    d_ptr->offsets.clear();
    return false;
  }

  char *data = (char *)d_ptr->file->map(0, size);
  if (data == 0) {
    d_ptr->offsets.clear();
    return false;
  }

  d_ptr->ptr = data;
  d_ptr->size = size;

  return true;
}

QString TextProvider::text(int aya, off_t start) const {
  const QPair<off_t, size_t>& a = d_ptr->offsets.at(start + aya);

  char *data = d_ptr->ptr + a.first;

  return QString::fromUtf8(data, a.second);
}

void TextProvider::unload() {
  if (!d_ptr->ptr) {
    return;
  }

  d_ptr->file->unmap((uchar *)d_ptr->ptr);
  d_ptr->file->close();
  d_ptr->ptr = 0;
}
