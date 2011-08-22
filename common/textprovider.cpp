#include "textprovider.h"
#include <QSettings>
#include "index.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <QDebug>

class TextProviderPrivate {
public:
  QList<QPair<off_t, size_t> > offsets;
  QString index;
  QString data;
  char *ptr;
  off_t size;
  int id;
};

TextProvider::TextProvider(int id, const QString& dataFile, const QString& indexFile)
  : d_ptr(new TextProviderPrivate) {
  d_ptr->size = 0;
  d_ptr->ptr = 0;
  d_ptr->index = indexFile;
  d_ptr->data = dataFile;
  d_ptr->id = id;
}

TextProvider::~TextProvider() {
  unload();

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

  int fd = open(d_ptr->data.toLocal8Bit().data(), O_RDONLY);
  if (fd == -1) {
    d_ptr->offsets.clear();
    return false;
  }

  struct stat buf;
  if (fstat(fd, &buf) != 0) {
    close(fd);
    d_ptr->offsets.clear();
    return false;
  }

  if (buf.st_size != size) {
    close(fd);
    d_ptr->offsets.clear();
    return false;
  }

  char *data = (char *)mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == MAP_FAILED) {
    close(fd);
    d_ptr->offsets.clear();
    return false;
  }

  d_ptr->ptr = data;
  d_ptr->size = size;

  close(fd);

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

  munmap(d_ptr->ptr, d_ptr->size);
  d_ptr->ptr = 0;
}
