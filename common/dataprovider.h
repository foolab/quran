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

#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <QObject>

class TextProvider;

class DataProvider : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString basmala READ basmala NOTIFY basmalaChanged);
  Q_PROPERTY(int pageCount READ pageCount CONSTANT);
  Q_PROPERTY(int partCount READ partCount CONSTANT);
  Q_PROPERTY(int chapterCount READ chapterCount CONSTANT);

public:
  DataProvider(QObject *parent = 0);
  ~DataProvider();

  int pageCount() const;
  int partCount() const;
  int chapterCount() const;

  Q_INVOKABLE bool setTextType(int index);
  Q_INVOKABLE int textType() const;

  QString basmala() const;

  QString text(int sura, int aya) const;

  QString secondaryText(int sura, int aya);

  void setSecondaryText(TextProvider *text);

  TextProvider *secondaryTextProvider() const;

signals:
  void basmalaChanged();
  void textTypeChanged();
  void secondaryTextProviderChanged();

private:
  int m_index;
  TextProvider *m_data;
  TextProvider *m_secondary;
};

#endif /* DATA_PROVIDER_H */
