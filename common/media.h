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

#ifndef MEDIA_H
#define MEDIA_H

#include <QUrl>

class Media {
public:
  Media(int chapter, int verse, const QUrl& url);
  virtual ~Media();

  int chapter() const;
  int verse() const;
  QUrl url() const;

  bool operator==(const Media& other);

private:
  int m_chapter;
  int m_verse;
  QUrl m_url;
};

#endif /* MEDIA_H */
