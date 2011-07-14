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

#include "pagepositioncontroller.h"

PagePositionController::PagePositionController(QObject *parent) :
  QObject(parent), m_sura(-1), m_aya(-1) {

}

PagePositionController::~PagePositionController() {

}

void PagePositionController::setSura(int sura) {
  if (m_sura != sura) {
    m_sura = sura;
    emit suraChanged();
  }
}

int PagePositionController::sura() const {
  return m_sura;
}

void PagePositionController::setAya(int aya) {
  if (m_aya != aya) {
    m_aya = aya;
    emit ayaChanged();
  }
}

int PagePositionController::aya() const {
  return m_aya;
}

bool PagePositionController::isValid() {
  return m_sura != -1 && m_aya != -1;
}

void PagePositionController::set(int sura, int aya) {
  m_sura = sura;
  m_aya = aya;

  emit suraChanged();
  emit ayaChanged();
}

void PagePositionController::reset() {
  set(-1, -1);
}
