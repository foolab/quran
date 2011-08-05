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

#include "dataprovider.h"
#include "metadata.h"
#include <QString>
#include <QDir>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <QDebug>

#define CLAMP(min, x, max) qMax(qMin(x, max), min)

DataProvider::DataProvider(const QString& dir, QObject *parent) :
  QObject(parent), m_dir(dir), m_index(-1), m_data(0) {
}

DataProvider::~DataProvider() {
  if (m_data) {
    munmap(m_data, Texts[m_index].len);
    m_data = 0;
    m_index = -1;
  }
}

int DataProvider::pageCount() {
  return MAX_PAGE + 1;
}

QStringList DataProvider::surasForPage(int page) {
  QList<Fragment> frags = pageFromIndex(page).fragments();

  QList<int> suras;

  QStringList ret;

  for (int x = 0; x < frags.size(); x++) {
    int sura = frags.at(x).sura();
    if (suras.indexOf(sura) == -1) {
      suras.append(sura);
    }
  }

  for (int x = 0; x < suras.size(); x++) {
    ret.append(sura(suras.at(x)).name());
  }

  return ret;
}

// TODO:
/*
QString DataProvider::firstSuraForPage(int page) {
  _Page *p = &Pages[page];
  int frag = p->firstFragment;
  int sura = Fragments[frag].sura;
  return QString::fromUtf8(Suras[sura].name);
}
*/

int DataProvider::suraCount() {
  return MAX_SURA + 1;
}

QString DataProvider::suraName(int sura) {
  return QString::fromUtf8(Suras[sura].name);
}

QString DataProvider::fullSuraName(int sura) {
  return QString("%1 %2").arg(prefix()).arg(suraName(sura));
}

int DataProvider::pageNumberForSura(int sura) {
  return Suras[sura].page;
}

int DataProvider::pageNumberForSuraAndAya(int sura, int aya) {
  int page = Suras[sura].page;

  _Page *p = &Pages[page];

  while (p->sura == sura) {
    if (p->aya == aya) {
      return p->index;
    }
    else if (p->aya > aya) {
      --p;
      return p->index;
    }

    ++p;
  }

  // We should never reach this but whatever.
  return page;
}

QString DataProvider::partName(int page) {
  _Page *p = &Pages[page];

  return QString::fromUtf8(Parts[p->part].name);
}

// TODO:
//int DataProvider::numberOfSuraPages(int sura) {
//  return Suras[sura].length;
//}



bool DataProvider::hasPage(int page) const {
  return page >= MIN_PAGE && page <= MAX_PAGE;
}

QString DataProvider::basmala() const {
  if (!m_data) {
    return QString();
  }

  return text(0, 0);
}

QString DataProvider::prefix() const {
  return QString::fromUtf8(PREFIX);
}

QString DataProvider::text(int sura, int aya) const {
  if (!m_data) {
    return QString();
  }

  off_t index = Offsets[sura];

  Aya *a = Texts[m_index].table + index + aya;

  char *data = m_data + a->pos;

  return QString::fromUtf8(data, a->len);
}

QStringList DataProvider::text(const Fragment& frag) const {
  if (!m_data) {
    return QStringList();
  }

  QStringList ret;

  for (int x = frag.start(); x < frag.size() + frag.start(); x++) {
    ret.append(text(frag.sura(), x));
  }

  return ret;
}

QStringList DataProvider::availableTexts() const {
  QStringList texts;

  for (int x = MIN_TEXT; x <= MAX_TEXT; x++) {
    texts.append(QString::fromUtf8(Texts[x].id));
  }

  return texts;
}

bool DataProvider::setText(int index) {
  int idx = CLAMP(MIN_TEXT, index, MAX_TEXT);
  if (idx != index) {
    return false;
  }

  if (idx == m_index) {
    return true;
  }

  QString file = QString("%1%2%3").arg(m_dir).arg(QDir::separator()).arg(Texts[idx].name);

  int fd = open(file.toLocal8Bit().data(), O_RDONLY);
  if (fd == -1) {
    return false;
  }

  struct stat buf;
  if (fstat(fd, &buf) != 0) {
    close(fd);
    return false;
  }

  if (buf.st_size != Texts[idx].len) {
    close(fd);
    return false;
  }

  char *data = (char *)mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == MAP_FAILED) {
    close(fd);
    return false;
  }

  if (m_data) {
    if (munmap(m_data, Texts[m_index].len) != 0) {
      close(fd);
      munmap(data, buf.st_size);
      return false;
    }
  }

  m_data = data;
  m_index = idx;

  close(fd);

  return true;
}

Sura DataProvider::sura(int index) const {
  return Sura(CLAMP(MIN_SURA, index, MAX_SURA));
}

Page DataProvider::pageFromIndex(int index) const {
  return Page(CLAMP(MIN_PAGE, index, MAX_PAGE));
}

Page DataProvider::pageForSura(int sura) const {
  return Page(Suras[CLAMP(MIN_SURA, sura, MAX_SURA)].page);
}

Page DataProvider::page(int sura, int aya) const {
  sura = CLAMP(MIN_SURA, sura, MAX_SURA);

  aya = CLAMP(0, aya, Suras[sura].length - 1);

  int page = Suras[sura].page;

  int result = -1;

  for (int x = page; x <= MAX_PAGE; x++) {
    _Page *p = &Pages[x];
    if (p->sura != sura) {
      break;
    }

    result = x;

    if (p->aya >= aya) {
      break;
    }
  }

  if (result == -1) {
    // Heck :(
    return pageForSura(sura);
  }

  return Page(result);
}

QList<Fragment> Page::fragments() {
  QList<Fragment> frags;

  _Page *p = &Pages[m_index];

  for (int x = p->firstFragment; x < p->firstFragment + p->fragments; x++) {
    frags << Fragment(CLAMP(MIN_FRAG, x, MAX_FRAG));
  }

  return frags;
}

int Fragment::sura() const {
  return Fragments[m_index].sura;
}

int Fragment::start() const {
  return Fragments[m_index].start;
}

int Fragment::size() const {
  return Fragments[m_index].size;
}

Sura::Sura(int index) : m_index(index) {

}

Sura::~Sura() {

}

QString Sura::name() const {
  return QString::fromUtf8(Suras[m_index].name);
}

bool Sura::hasBasmala() const {
  return !(m_index == 0 || m_index == 8);
}

int Sura::size() const {
  return Suras[m_index].length;
}

int Sura::index() const {
  return m_index;
}