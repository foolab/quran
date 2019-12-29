/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef INTENT_H
#define INTENT_H

#include <QAndroidIntent>

class Intent : public QAndroidIntent {
public:
  using QAndroidIntent::QAndroidIntent;

  void putExtraString(const QString& key, const QString& data);
  //  QString extraString(const QString& key);

  void setAction(const QString& action);
  QString action();
};

#endif /* BINDER_H */
