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
#include "chapterinfo.h"

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
    ret.append(ChapterInfo(suras.at(x)).name());
  }

  return ret;
}

int DataProvider::firstSuraForPage(int page) {
  return (&Pages[page])->sura;
}

int DataProvider::chapterCount() const {
  return MAX_CHAPTER + 1;
}

int DataProvider::pageNumberForSuraAndAya(int sura, int aya) {
  int page = ChapterInfo(sura).page();
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

  return m_data->text(aya, ChapterInfo(sura).offset());
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
    ret << m_secondary->text(x, ChapterInfo(frag.sura()).offset());
  }

  return ret;
}

QString DataProvider::secondaryText(int sura, int aya) {
  if (!m_secondary) {
    return QString();
  }

  return m_secondary->text(aya, ChapterInfo(sura).offset());
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

Page DataProvider::pageFromIndex(int index) const {
  return Page(CLAMP(MIN_PAGE, index, MAX_PAGE));
}

Page DataProvider::pageForSura(int sura) const {
  return Page(ChapterInfo(CLAMP(MIN_CHAPTER, sura, MAX_CHAPTER)).page());
}

Page DataProvider::page(int sura, int aya) const {
  sura = CLAMP(MIN_CHAPTER, sura, MAX_CHAPTER);

  aya = CLAMP(0, aya, ChapterInfo(sura).length() - 1);

  int page = ChapterInfo(sura).page();

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

  return ChapterInfo(sura).length();
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
