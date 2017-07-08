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

#include "dataprovider.h"
#include "metadata.h"
#include <QString>
#include <QDebug>
#include "textprovider.h"

#define MIN_CHAPTER 0
#define MAX_CHAPTER 113
#define MIN_PAGE 0
#define MAX_PAGE 603
#define MIN_PART 0
#define MAX_PART 29

#define CLAMP(min, x, max) qMax(qMin(x, max), min)

DataProvider::DataProvider(QObject *parent) :
  QObject(parent),
  m_index(-1),
  m_data(0),
  m_secondary(0) {

}

DataProvider::~DataProvider() {
  if (m_data) {
    delete m_data;
    m_data = 0;
    m_index = -1;
  }

  if (m_secondary) {
    delete m_secondary;
    m_secondary = 0;
  }
}

int DataProvider::pageCount() const {
  return MAX_PAGE + 1;
}

int DataProvider::partCount() const {
  return MAX_PART + 1;
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

int DataProvider::firstSuraForPage(int page) {
  return (&Pages[page])->sura;
}

int DataProvider::verseCount() const {
  return MAX_CHAPTER + 1;
}

QString DataProvider::suraName(int sura) {
  return QString::fromUtf8(Suras[sura].name);
}

QString DataProvider::fullSuraName(int sura) {
  return QString("%1 %2").arg(prefix()).arg(suraName(sura));
}

QString DataProvider::translatedSuraName(int sura) {
  return QString::fromUtf8(Suras[sura].translation);
}

QString DataProvider::transliteratedSuraName(int sura) {
  return QString::fromUtf8(Suras[sura].transliteration);
}

int DataProvider::pageNumberForSura(int sura) {
  return Suras[sura].page;
}

int DataProvider::pageNumberForSuraAndAya(int sura, int aya) {
  int page = Suras[sura].page;
  _Page *p = &Pages[page];

  for (int x = p->firstFragment; x <= MAX_FRAG; x++) {
    _Fragment *f = &Fragments[x];
    if (f->sura == sura && f->start <= aya && f->start + f->size > aya) {
      return f->page;
    }
  }

  // We shouldn't reach this but you never know!
  return p->index;
}

QString DataProvider::partNameForPage(int page) {
  _Page *p = &Pages[page];

  return QString::fromUtf8(Parts[p->part].name);
}

QString DataProvider::partName(int part) {
  return QString::fromUtf8(Parts[part].name);
}

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

  return m_data->text(aya, index);
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

QStringList DataProvider::secondaryText(const Fragment& frag) const {
  if (!m_secondary) {
    return QStringList();
  }

  QStringList ret;

  for (int x = frag.start(); x < frag.size() + frag.start(); x++) {
    off_t index = Offsets[frag.sura()];
    ret << m_secondary->text(x, index);
  }

  return ret;
}

QString DataProvider::secondaryText(int sura, int aya) {
  if (!m_secondary) {
    return QString();
  }

  off_t index = Offsets[sura];
  return m_secondary->text(aya, index);
}

QStringList DataProvider::availableTexts() const {
  QStringList texts;

  for (int x = MIN_TEXT; x <= MAX_TEXT; x++) {
    texts.append(QString::fromUtf8(Texts[x].id));
  }

  return texts;
}

int DataProvider::textType() const {
  return m_index;
}

bool DataProvider::setTextType(int index) {
  if (index != CLAMP(MIN_TEXT, index, MAX_TEXT)) {
    return false;
  }

  if (index == m_index) {
    return true;
  }

  QString data = Texts[index].name;
  QString idx = Texts[index].idx;

  TextProvider *p = new TextProvider(index, data, idx);
  if (!p->load()) {
    delete p;
    return false;
  }

  if (m_data) {
    delete m_data;
  }

  m_data = p;

  emit basmalaChanged();

  emit textTypeChanged();

  return true;
}

Sura DataProvider::sura(int index) const {
  return Sura(CLAMP(MIN_CHAPTER, index, MAX_CHAPTER));
}

Page DataProvider::pageFromIndex(int index) const {
  return Page(CLAMP(MIN_PAGE, index, MAX_PAGE));
}

Page DataProvider::pageForSura(int sura) const {
  return Page(Suras[CLAMP(MIN_CHAPTER, sura, MAX_CHAPTER)].page);
}

Page DataProvider::page(int sura, int aya) const {
  sura = CLAMP(MIN_CHAPTER, sura, MAX_CHAPTER);

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

TextProvider *DataProvider::secondaryTextProvider() const {
  return m_secondary;
}

void DataProvider::setSecondaryText(TextProvider *text) {
  if (m_secondary != text) {
    delete m_secondary;
    m_secondary = text;
    emit secondaryTextProviderChanged();
  }
}

bool DataProvider::hasBasmala(int sura) {
  return !(sura == 0 || sura == 8);
}

int DataProvider::pageNumberForPart(int part) {
  part = qBound(MIN_PART, part, MAX_PART);

  for (int x = 0; x <= MAX_PAGE; x++) {
    if (Pages[x].part == part) {
      return x;
    }
  }

  // Heck!
  return 0;
}

int DataProvider::partNumberForPage(int page) {
  page = qBound(MIN_PAGE, page, MAX_PAGE);

  return Pages[page].part;
}

int DataProvider::suraSize(int sura) {
  if (sura < 0 || sura > 113) {
    return 0;
  }

  return Suras[sura].length;
}


QList<Fragment> DataProvider::fragmentsForPart(int part) {
  QList<Fragment> frags;

  for (int x = 0; x <= MAX_PAGE; x++) {
    if (Pages[x].part == part) {
      frags += pageFromIndex(x).fragments();
    }
    else if (!frags.isEmpty()) {
      // We filled our list already
      break;
    }
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
