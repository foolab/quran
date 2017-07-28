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


#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QDebug>
#include <QScopedPointer>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "index.h"
#include <unistd.h>

struct Sura {
  QString name;
  QString translation;
  QString transliteration;
  int start;
  int end;
  int ayas;
  int index;
  int page;
};
QList<Sura> suras;

struct Page {
  int index;
  int sura;
  int aya;
  int part;
  int firstFragment;
  int fragments;
};
QList<Page> pages;

struct Fragment {
  int index;
  int sura;
  int page;
  int start;
  int size;
};
QList<Fragment> frags;

struct Offset {
  QString name;
  QString id;
  QString idx;
  QList<QPair<off_t, size_t> > index;
  size_t len;
};
QList<Offset> offsets;

struct Part {
  QString name;
  int index;
  int sura;
  int aya;
};
QList<Part> parts;

QString basmala;

bool calcOffsets(const QString& id, const QString& fileName, const QString& idx) {
  int fd = open(fileName.toLocal8Bit(), O_RDONLY);
  if (fd == -1) {
    perror("open");
    return false;
  }

  struct stat buf;
  if (fstat(fd, &buf) != 0) {
    perror("stat");
    close(fd);
    return false;
  }

  char *data = (char *)mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == MAP_FAILED) {
    perror("mmap");
    close(fd);
    return false;
  }

  Offset o;
  o.idx = idx;
  o.id = id;
  o.name = QFileInfo(fileName).fileName();
  o.len = buf.st_size;

  char *start = data;

  if (basmala.isEmpty()) {
    const char *_b = "bismillah=\"";
    char *b = strstr(start, _b);
    if (!b) {
      qDebug() << "Basmala not found!";
      return false;
    }
    b += strlen(_b);
    char *e = strstr(b, "\"");
    if (!e) {
      qDebug() << "Basmala end not found!";
      return false;
    }

    basmala = QString::fromUtf8(b, e - b);
  }

  for (int x = 0; x < suras.size(); x++) {
    QList<QPair<off_t, size_t> > lst;

    for (int i = 0; i < suras.at(x).ayas; i++) {
      char *f = strstr(start, "text=\"");
      if (!f) {
	qDebug() << "Cannot find start element";
	return false;
      }

      f += 6;

      char *ff = strstr(f, "\"");
      if (!ff) {
	munmap(data, buf.st_size);
	close(fd);
	qDebug() << "Cannot find end element";
	return false;
      }

      start = ff;
      off_t off = f - data;
      size_t len = ff - f;
      std::string str(&data[off], len);

      lst.append(qMakePair<off_t, size_t>(off, len));
    }

    o.index.append(lst);
  }

  munmap(data, buf.st_size);
  close(fd);

  offsets.append(o);

  return true;
}

void calcFragmentsPerPage() {
  for (int x = 0; x < frags.size(); x++) {
    const Fragment& f = frags.at(x);
    Page& p = pages[f.page];
    if (p.firstFragment == -1) {
      p.firstFragment = f.index;
      p.fragments = 1;
    }
    else {
      p.fragments++;
    }
  }
}

void calcPagePerSura() {
  for (int x = 0; x < frags.size(); x++) {
    int sura = frags.at(x).sura;
    if (suras[sura].page == -1) {
      int page = frags.at(x).page;
      suras[sura].page = page;
    }
  }
}

void calcFragments(Page& c, Page& n) {
  if (c.sura == n.sura) {
    Fragment f;
    f.sura = c.sura;
    f.index = frags.size();
    f.page = c.index;
    f.start = c.aya;
    f.size = n.aya - c.aya;
    frags << f;

    return;
  }

  for (int x = c.sura; x < n.sura; x++) {
    Fragment f;
    f.sura = x;
    f.index = frags.size();
    f.page = c.index;
    f.start = (x == c.sura ? c.aya : 0);
    f.size = suras.at(x).ayas - f.start;

    frags << f;
  }

  if (n.aya != 0) {
    Fragment f;
    f.sura = n.sura;
    f.index = frags.size();
    f.page = c.index;
    f.start = 0;
    f.size = n.aya;

    frags << f;
  }
}

void calcFragments() {
  for (int x = 0; x < pages.size() - 1; x++) {
    Page& c = pages[x];
    Page& n = pages[x + 1];
    calcFragments(c, n);
  }

  Page fake;
  fake.aya = 0;
  fake.index = 604;
  fake.sura = 114;
  calcFragments(pages.last(), fake);
}

QFile *open(char *f) {
  QScopedPointer<QFile> file(new QFile(f));
  if (!file->open(QFile::ReadOnly)) {
    qDebug() << "Failed to open" << f << file->errorString();
    return 0;
  }

  return file.take();
}

bool readMetaData(char *f) {
  QScopedPointer<QFile> file(open(f));

  if (!file) {
    return false;
  }

  QXmlStreamReader reader(file.data());
  while (!reader.atEnd()) {
    reader.readNext();
    if (reader.isStartElement() && reader.name() == "sura") {
      QXmlStreamAttributes attrs(reader.attributes());
      if (!attrs.hasAttribute("ayas") || !attrs.hasAttribute("start")
	  || !attrs.hasAttribute("index") || !attrs.hasAttribute("name")
	  || !attrs.hasAttribute("ename") || !attrs.hasAttribute("tname")) {
	qCritical() << "Missing attributes for sura";
	return false;
      }

      Sura sura;
      sura.name = attrs.value("name").toString().trimmed();
      sura.translation = attrs.value("ename").toString().trimmed();
      sura.transliteration = attrs.value("tname").toString().trimmed();
      sura.index = attrs.value("index").toString().toInt() - 1;
      sura.ayas = attrs.value("ayas").toString().toInt();
      sura.start = attrs.value("start").toString().toInt();
      sura.end = sura.ayas + sura.start - 1;
      sura.page = -1;

      suras << sura;
    }
    else if (reader.isStartElement() && reader.name() == "page") {
      QXmlStreamAttributes attrs(reader.attributes());
      if (!attrs.hasAttribute("index") || !attrs.hasAttribute("aya")
	  || !attrs.hasAttribute("sura")) {
	qCritical() << "Missing attributes for page";
	return false;
      }

      Page page;
      page.index = attrs.value("index").toString().toInt() - 1;
      page.sura = attrs.value("sura").toString().toInt() - 1;
      page.aya = attrs.value("aya").toString().toInt() - 1;
      page.fragments = -1;
      page.firstFragment = -1;
      page.part = -1;

      pages << page;
    }
    else if (reader.isStartElement() && reader.name() == "juz") {
      QXmlStreamAttributes attrs(reader.attributes());
      if (!attrs.hasAttribute("index") || !attrs.hasAttribute("aya")
	  || !attrs.hasAttribute("sura")) {
	qCritical() << "Missing attributes for juz!";
	return false;
      }

      Part part;
      part.index = attrs.value("index").toString().toInt() - 1;
      part.sura = attrs.value("sura").toString().toInt() - 1;
      part.aya = attrs.value("aya").toString().toInt() - 1;

      parts << part;
    }
  }
  if (reader.hasError()) {
    qCritical() << "Failed to read" << f << reader.errorString();
    return false;
  }

  return true;
}

bool ensureSuras() {
  if (suras.count() != 114) {
    qCritical() << "Suras are not 114";
    return false;
  }

  for (int x = 0; x < suras.count(); x++) {
    Sura& s = suras[x];

    if (s.index != x || (s.start == 0 && s.index != 0) || s.ayas == 0 || s.name.isEmpty()) {
      qCritical() << "Discrepancy in sura";
      return false;
    }
  }

  return true;
}

bool ensurePages() {
  if (pages.count() != 604) {
    qCritical() << "Pages are not 604";
    return false;
  }

  for (int x = 0; x < pages.size(); x++) {
    Page& p = pages[x];
    if (p.index != x || p.sura < 0 || p.aya < 0) {
      qCritical() << "Discrepancy in page";
      return false;
    }
  }

  return true;
}

int partForPage(int page) {
  int sura = pages.at(page).sura;
  int aya = pages.at(page).aya;

  for (int x = 0; x < parts.size(); x++) {
    const Part& p = parts.at(x);
    if (p.aya == aya && p.sura == sura) {
      return x;
    }
  }

  return -1;
}

bool calcParts() {
  QFile f("parts.txt");
  if (!f.open(QFile::ReadOnly)) {
    qCritical() << "Failed to open" << f.fileName() << f.errorString();
    return false;
  }

  for (int x = 0; x < parts.size(); x++) {
    QByteArray a = f.readLine();
    if (a.isEmpty()) {
      qCritical() << "Failed to read" << f.fileName() << f.errorString();
      return false;
    }

    while (a.endsWith('\n') || a.endsWith('\r')) {
      a.chop(1);
    }

    QString s = QString::fromUtf8(a);

    if (s.isEmpty()) {
      qCritical() << "Malformed parts file";
      return false;
    }

    parts[x].name = s;
  }

  // Now for pages:
  int last = -1;
  for (int x = 0; x < pages.size(); x++) {
    Page& p = pages[x];
    int part = partForPage(x);
    if (part == -1 && last == -1) {
      qCritical() << "Failed to find part for page" << x;
      return false;
    }

    if (part == -1) {
      p.part = last;
    }
    else {
      p.part = part;
      last = part;
    }
  }

  return true;
}

QString encode(const QString& in) {
  QByteArray a(in.toUtf8());
  const char *data = a.constData();
  QString out;
  QTextStream s(&out);

  while (*data) {
    s << "\\x" << QByteArray(1, *data).toHex();
    ++data;
  }

  return out;
}

bool output() {
  QString prefix;

  prefix.append(QChar(0x633)).append(QChar(0x648)).append(QChar(0x631)).append(QChar(0x629));

  puts("#ifndef TEXT_META_H");
  puts("#define TEXT_META_H");
  puts("");
  puts("// Automatically generated file. Do not edit!");
  puts("// Command:");
  printf("// %s\n", QCoreApplication::arguments().join(" ").toUtf8().data());
  puts("");
#if 0
  printf("#define MIN_FRAG %i\n", frags.first().index);
  printf("#define MAX_FRAG %i\n", frags.last().index);
#endif
  printf("#define MIN_TEXT %i\n", 0);
  printf("#define MAX_TEXT %i\n", offsets.size() - 1);
#if 0
  puts("");
  printf("#define PREFIX \"%s\"\n", encode(prefix).toLatin1().data());
  puts("");
  puts("struct _Sura {");
  puts("  int index;");
  puts("  int length;");
  puts("  int page;");
  puts("  const char *name;");
  puts("  const char *translation;");
  puts("  const char *transliteration;");
  puts("} Suras[] = {");
  for (int x = 0; x < suras.size(); x++) {
    const Sura& s = suras.at(x);
    printf("  {%i, %i, %i, \"%s\", \"%s\", \"%s\"},\n", s.index, s.ayas, s.page, encode(s.name).toLatin1().data(), encode(s.translation).toLatin1().data(), encode(s.transliteration).toLatin1().data());
  }
  puts("};");

  puts("");

  puts("struct _Part {");
  puts("  int index;");
  puts("  const char *name;");
  puts("} Parts[] = {");
  for (int x = 0; x < parts.size(); x++) {
    const Part& p = parts.at(x);
    printf("  {%i, \"%s\"},\n", p.index, encode(p.name).toLatin1().data());
  }
  puts("};");

  puts("");

  puts("struct _Page {");
  puts("  int index;");
  puts("  int sura;");
  puts("  int aya;");
  puts("  int firstFragment;");
  puts("  int fragments;");
  puts("  int part;");
  puts("} Pages[] = {");
  for (int x = 0; x < pages.size(); x++) {
    const Page& p = pages.at(x);
    printf("  {%i, %i, %i, %i, %i, %i},\n", p.index, p.sura, p.aya, p.firstFragment, p.fragments, p.part);
  }
  puts("};");

  puts("");

  puts("struct _Fragment {");
  puts("  int index;");
  puts("  int sura;");
  puts("  int page;");
  puts("  int start;");
  puts("  int size;");
  puts("} Fragments[] = {");
  for (int x = 0; x < frags.size(); x++) {
    const Fragment& f = frags.at(x);
    printf("  {%i, %i, %i, %i, %i},\n", f.index, f.sura, f.page, f.start, f.size);
  }
  puts("};");
#endif
  puts("");

  for (int x = 0; x < offsets.size(); x++) {
    const Offset& o = offsets.at(x);

    QMap<QString, QVariant> meta;
    meta.insert("size", QVariant::fromValue(o.len));

    if (!Index::write(o.idx, o.index, meta)) {
      qCritical() << "Failed to write index";
      return false;
    }
  }

  puts("struct _Text {");
  puts("  const char *name;");
  puts("  const char *id;");
  puts("  off_t len;");
  puts("  const char *idx;");
  puts("} Texts[] = {");
  for (int x = 0; x < offsets.size(); x++) {
    const Offset& o = offsets.at(x);
    printf("{\"%s\", \"%s\", %i, \"%s\"},\n",
	   encode(":/text/" + o.name).toLatin1().data(), encode(o.id).toLatin1().data(),
	   o.len, encode(":/text/" + QFileInfo(o.idx).fileName()).toLatin1().data());
  }
  puts("};");

  puts("");
#if 0
  puts("int Offsets[] = {");
  off_t total = 0;
  for (int x = 0; x < suras.size(); x++) {
    printf("  %li,\n", total);
    total += suras[x].ayas;
  }
  puts("};");
#endif
  puts("");

  puts("#endif /* TEXT_META_H */");

  return true;
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  Q_UNUSED(app);

  if (argc < 3) {
    qCritical() << "Usage: <meta data.xml> <file1.xml> ...";
    return 1;
  }

  if (!readMetaData(argv[1]) || !ensureSuras() || !ensurePages()) {
    return 1;
  }

  calcFragments();

  calcPagePerSura();

  calcFragmentsPerPage();

  if (!calcParts()) {
    return 1;
  }

  for (int x = 2; x < argc; x++) {
    QStringList parts = QString(argv[x]).split(":");
    if (parts.size() != 3) {
      qWarning() << "invalid file";
      return 1;
    }

    if (!calcOffsets(parts[0], parts[1], parts[2])) {
      return 1;
    }
  }

  if (!output()) {
    return 1;
  }

  return 0;
}
