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

#include "media.h"

Media::Media(int chapter, int verse, int index, const QUrl& url) :
  m_chapter(chapter),
  m_verse(verse),
  m_index(index),
  m_url(url) {

}

Media::~Media() {

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

QUrl Media::url() const {
  return m_url;
}
