/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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

#include "media.h"
#include "recitation.h"

Media::Media(Recitation *recitation, int chapter, int verse, int index, bool signal) :
  m_recitation(recitation),
  m_chapter(chapter),
  m_verse(verse),
  m_index(index),
  m_signal(signal) {

}

Media::~Media() {

}

Media Media::error() {
  return Media(0, 0, 0, -2, false);
}

Media Media::eos() {
  return Media(0, 0, 0, -1, false);
}

bool Media::isError() {
  return m_index == -2;
}

bool Media::isEos() {
  return m_index == -1;
}

int Media::chapter() const {
  return m_chapter;
}

int Media::verse() const {
  return m_verse;
}

int Media::index() const {
  return m_index;
}

bool Media::signal() const {
  return m_signal;
}

QByteArray Media::data() const {
  return m_recitation->data(*this);
}

bool Media::setData(const QByteArray& data) const {
  return m_recitation->setData(*this, data);
}
