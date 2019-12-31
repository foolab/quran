/*
 * Copyright (c) 2014 Mohammed Sameer <msameer@foolab.org>.
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

#include "textsupplier.h"
#include "dataprovider.h"

TextSupplier::TextSupplier(QObject *parent) :
  QObject(parent),
  m_data(0),
  m_chapter(-1),
  m_verse(-1) {

}

TextSupplier::~TextSupplier() {

}

void TextSupplier::setData(DataProvider *data) {
  if (m_data != data) {

    if (m_data) {
      QObject::disconnect(m_data, SIGNAL(textTypeChanged()), this, SLOT(recheckPrimary()));
      QObject::disconnect(m_data, SIGNAL(secondaryTextProviderChanged()), this, SLOT(recheckSecondary()));
    }

    m_data = data;

    if (m_data) {
      QObject::connect(m_data, SIGNAL(textTypeChanged()), this, SLOT(recheckPrimary()));
      QObject::connect(m_data, SIGNAL(secondaryTextProviderChanged()), this, SLOT(recheckSecondary()));
    }

    emit dataChanged();
    recheck();
  }
}

DataProvider *TextSupplier::data() const {
  return m_data;
}

int TextSupplier::chapter() const {
  return m_chapter;
}

void TextSupplier::setChapter(int chapter) {
  if (m_chapter != chapter) {
    m_chapter = chapter;
    emit chapterChanged();
    recheck();
  }
}

int TextSupplier::verse() const {
  return m_verse;
}

void TextSupplier::setVerse(int verse) {
  if (m_verse != verse) {
    m_verse = verse;
    emit verseChanged();
    recheck();
  }
}

QString TextSupplier::primaryText() const {
  return m_data && m_verse != -1 && m_chapter != -1 ? m_data->text(m_chapter, m_verse) : QString();
}

QString TextSupplier::secondaryText() const {
  return m_data && m_verse != -1 && m_chapter != -1 ? m_data->secondaryText(m_chapter, m_verse) : QString();
}

void TextSupplier::recheck() {
  recheckPrimary();
  recheckSecondary();
}

void TextSupplier::recheckPrimary() {
  if (m_data && m_verse != -1 && m_chapter != -1) {
    emit primaryTextChanged();
  }
}

void TextSupplier::recheckSecondary() {
  if (m_data && m_verse != -1 && m_chapter != -1) {
    emit secondaryTextChanged();
  }
}
