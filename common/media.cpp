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

Media::Media(int chapter, int verse, const QUrl& url) :
  m_chapter(chapter),
  m_verse(verse),
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

QUrl Media::url() const {
  return m_url;
}

bool Media::operator==(const Media& other) {
  return m_url == other.m_url && m_chapter == other.m_chapter && m_verse == other.m_verse;
}

