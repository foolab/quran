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

#include "translation.h"
#include "translations.h"
#include "translation_p.h"
#include <QDebug>

Translation::Translation(QObject *parent)
  : QObject(parent), m_tid(-1),
    d_ptr(0), m_translations(0) {
}

Translation::~Translation() {
  if (m_translations) {
    m_translations->unregisterTranslation(this);
  }

  d_ptr = 0;
}

void Translation::init() {
  d_ptr = m_translations->registerTranslation(this);
}

void Translation::setTid(int tid) {
  if (m_tid != tid) {
    m_tid = tid;

    emit tidChanged();
  }
}

int Translation::tid() const {
  return m_tid;
}

int Translation::downloadProgress() const {
  return d_ptr ? d_ptr->downloadProgress() : 0;
}

Translation::Status Translation::status() const {
  return d_ptr ? d_ptr->status() : Translation::None;
}

QString Translation::error() const {
  return d_ptr ? d_ptr->error() : QString();
}

void Translation::setTranslations(Translations *translations) {
  m_translations = translations;
}

Translations *Translation::translations() const {
  return m_translations;
}
