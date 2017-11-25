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

#ifndef MODELS_H
#define MODELS_H

#include <QSortFilterProxyModel>
#include <QAbstractListModel>

class Translations;
class Translation;
class Recitations;
class Recitation;

class VisibilityFilterModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged);

public:
  VisibilityFilterModel(QObject *parent = 0);
  ~VisibilityFilterModel();

  QAbstractItemModel *model();
  void setModel(QAbstractItemModel *model);

signals:
  void modelChanged();

protected:
  virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
};

class TranslationsModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(Translations *source READ source WRITE setSource NOTIFY sourceChanged);

public:
  enum {
    TranslationRole = Qt::UserRole,
    SectionRole = Qt::UserRole + 1,
    VisibleRole = Qt::UserRole + 2,
  };

  TranslationsModel(QObject *parent = 0);
  ~TranslationsModel();

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

class RecitationsModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(Recitations *source READ source WRITE setSource NOTIFY sourceChanged);

public:
  enum {
    RecitationRole = Qt::UserRole,
    SectionRole = Qt::UserRole + 1,
    VisibleRole = Qt::UserRole + 2,
  };

  RecitationsModel(QObject *parent = 0);
  ~RecitationsModel();

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
