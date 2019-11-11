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

#ifndef MODELS_H
#define MODELS_H

#include <QSortFilterProxyModel>
#include <QAbstractListModel>

class Translations;
class Translation;
class Recitations;
class Recitation;

class LessThanInterface {
public:
  virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) = 0;
};

class SortModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged);

public:
  SortModel(QObject *parent = 0);
  ~SortModel();

  QAbstractItemModel *model();
  void setModel(QAbstractItemModel *model);

signals:
  void modelChanged();

protected:
  bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const;
};

class TranslationsModel : public QAbstractListModel, public LessThanInterface {
  Q_OBJECT
  Q_PROPERTY(Translations *source READ source WRITE setSource NOTIFY sourceChanged);

public:
  TranslationsModel(QObject *parent = 0);
  ~TranslationsModel();

  bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Translations *source() const;
  void setSource(Translations *translations);

  QHash<int, QByteArray> roleNames() const;

signals:
  void sourceChanged();

private slots:
  void refresh();
  void translationChanged(Translation *t);

private:
  Translations *m_translations;
};

class RecitationsModel : public QAbstractListModel, public LessThanInterface {
  Q_OBJECT
  Q_PROPERTY(Recitations *source READ source WRITE setSource NOTIFY sourceChanged);

public:
  RecitationsModel(QObject *parent = 0);
  ~RecitationsModel();

  bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Recitations *source() const;
  void setSource(Recitations *translations);

  QHash<int, QByteArray> roleNames() const;

signals:
  void sourceChanged();

private slots:
  void refresh();
  void recitationChanged(Recitation *r);

private:
  Recitations *m_recitations;
};

#endif /* MODELS_H */
