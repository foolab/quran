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

#ifndef METADATA_H
#define METADATA_H

#define MIN_CHAPTER 0
#define MAX_CHAPTER 113

#define MIN_PAGE 0
#define MAX_PAGE 603

#define MIN_PART 0
#define MAX_PART 29

#define MIN_FRAGMENT 0
#define MAX_FRAGMENT 661

#define CHAPTER "\xd8\xb3\xd9\x88\xd8\xb1\xd8\xa9"

struct Chapter {
  int index;
  int length;
  int page;
  int offset;
  const char *name;
  const char *translation;
  const char *transliteration;
};

Chapter *chapter(int index);

struct Part {
  int index;
  int firstPage;
  int numberOfPages;
  const char *name;
};

Part *part(int index);

struct Page {
  int index;
  int chapter;
  int verse;
  int firstFragment;
  int fragments;
  int part;
};

Page *page(int index);

struct Fragment {
  int index;
  int chapter;
  int page;
  int verse;
  int length;
};

Fragment *fragment(int index);

#endif /* METADATA_H */
