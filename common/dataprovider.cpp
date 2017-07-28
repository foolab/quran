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

#include "dataprovider.h"
#include "metadata.h"
#include <QString>
#include <QDebug>
#include "metadata.h"
#include "text-meta.h"

#include "textprovider.h"
#include "chapterinfo.h"
#include "partinfo.h"
#include "pageinfo.h"

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

int DataProvider::chapterCount() const {
  return MAX_CHAPTER + 1;
}

QString DataProvider::basmala() const {
  if (!m_data) {
    return QString();
  }

  return text(0, 0);
}

QString DataProvider::text(int sura, int aya) const {
  if (!m_data) {
    return QString();
  }

  return m_data->text(aya, ChapterInfo(sura).offset());
}

QString DataProvider::secondaryText(int sura, int aya) {
  if (!m_secondary) {
    return QString();
  }

  return m_secondary->text(aya, ChapterInfo(sura).offset());
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
