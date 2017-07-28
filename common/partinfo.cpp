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

#include "partinfo.h"
#include "metadata.h"

PartInfo::PartInfo(int index) {
  init(index);
}

PartInfo::PartInfo() :
  m_part(0) {

}

PartInfo::~PartInfo() {
  m_part = 0;
}

void PartInfo::init(int index) {
  m_part = part(index);
}

int PartInfo::index() const {
  return m_part ? m_part->index : -1;
}

int PartInfo::firstPage() const {
  return m_part ? m_part->firstPage : -1;
}

int PartInfo::numberOfPages() const {
  return m_part ? m_part->numberOfPages : -1;
}

QString PartInfo::name() const {
  return m_part ? QString::fromUtf8(m_part->name) : QString();
}

DeclarativePartInfo::DeclarativePartInfo(QObject *parent) :
  QObject(parent) {

}

int DeclarativePartInfo::part() const {
  return index();
}

void DeclarativePartInfo::setPart(int part) {
  if (index() != part) {
    init(part);
    emit partChanged();
  }
}
