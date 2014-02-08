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

#ifndef TRANSLATION_MODELS_H
#define TRANSLATION_MODELS_H

#include <QAbstractListModel>

class Translations;

class TranslationModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(Translations *translations READ translations WRITE setTranslations NOTIFY translationsChanged);

public:
  enum Roles {
    IdRole = Qt::UserRole + 1,
    NameRole,
  };

  TranslationModel(QObject *parent = 0);
  virtual ~TranslationModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Translations *translations() const;
  void setTranslations(Translations *translations);

signals:
  void translationsChanged();

protected slots:
  void addId(int id);
  void removeId(int id);

protected:
  virtual void translationsUpdated() = 0;

  void setIds(const QList<int>& ids);

  Translations *m_translations;
  bool m_categories;

private:
  void clear();

  QList<int> m_ids;
};

class InstalledTranslationsModel : public TranslationModel {
  Q_OBJECT

public:
  InstalledTranslationsModel(QObject *parent = 0);
  ~InstalledTranslationsModel();

private slots:
  void refresh();

protected:
  void translationsUpdated();
};

class ActiveTranslationsModel : public TranslationModel {
  Q_OBJECT

public:
  ActiveTranslationsModel(QObject *parent = 0);
  ~ActiveTranslationsModel();

private slots:
  void activeChanged();

protected:
  void translationsUpdated();
};

class TranslationCategoriesModel : public TranslationModel {
  Q_OBJECT

public:
  TranslationCategoriesModel(QObject *parent = 0);
  ~TranslationCategoriesModel();

protected:
  void translationsUpdated();
};

class TranslationCollection : public TranslationModel {
  Q_OBJECT
  Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged);

public:
  TranslationCollection(QObject *parent = 0);
  ~TranslationCollection();

  int language() const;
  void setLanguage(int language);

signals:
  void languageChanged();

protected:
  void translationsUpdated();

private:
  int m_language;
};

#endif /* TRANSLATION_MODELS_H */
