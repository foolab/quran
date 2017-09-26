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

#include "normalize.h"
#include <QList>
#include <QVector>

// yeh hamza and waw hamza -> yeh or waw + hamza
// various forms of alef -> alef
// alef maksora -> ya2
// hamza -> alef
// ta2 marbota -> heh

#define WAW_HAMZE 0x0624 // 0624;ARABIC LETTER WAW WITH HAMZA ABOVE
#define YEH_HAMZA 0x0626 // 0626;ARABIC LETTER YEH WITH HAMZA ABOVE
#define WAW 0x0648 // 0648;ARABIC LETTER WAW
#define HAMZA 0x0621 // 0621;ARABIC LETTER HAMZA
#define YEH 0x064A // 064A;ARABIC LETTER YEH
#define ALEF_MADDA_ABOVE 0x0622 // 0622;ARABIC LETTER ALEF WITH MADDA ABOVE
#define ALEF_HAMZA_ABOVE 0x0623 // 0623;ARABIC LETTER ALEF WITH HAMZA ABOVE
#define ALEF_HAMZA_BELOW 0x0625 // 0625;ARABIC LETTER ALEF WITH HAMZA BELOW
#define ALEF 0x0627 // 0627;ARABIC LETTER ALEF
#define ALEF_MAKSURA 0x0649 // 0649;ARABIC LETTER ALEF MAKSURA
#define TEH_MARBUTA 0x0629 // 0629;ARABIC LETTER TEH MARBUTA
#define HEH 0x0647 // 0647;ARABIC LETTER HEH

typedef struct {
  const QChar original;
  QList<QChar> replacements;
} Set;

QVector<Set> sets = {
  // waw hamza -> waw + hamza
  { WAW_HAMZE, {WAW, HAMZA} },
  // yeh hamza -> yeh + hamza
  { YEH_HAMZA, {YEH, HAMZA} },
  // various forms of alef -> alef
  { ALEF_MADDA_ABOVE, { ALEF } },
  { ALEF_HAMZA_ABOVE, { ALEF } },
  { ALEF_HAMZA_BELOW, { ALEF } },
  // alef maksora -> yeh
  { ALEF_MAKSURA, { YEH } },
  // hamza -> alef
  { HAMZA, { ALEF } },
  // ta2 marbota -> heh
  { TEH_MARBUTA, { HEH } },
};

QString normalize(const QChar& in) {
  for (const Set& set : sets) {
    if (set.original == in) {

      QString out;
      for (const QChar& c : set.replacements) {
	out.append(c);
      }

      return out;
    }
  }

  return in;
}

QString Normalize::normalize(const QString& in) {
  QString out;
  for (int x = 0; x < in.size(); x++) {
    QChar c = in[x];
    out.append(normalize(c));
  }

  return out;
}
