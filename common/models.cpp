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

#include "models.h"
#include "translations.h"
#include <QDebug>

TranslationModel::TranslationModel(QObject *parent) :
  QAbstractListModel(parent),
  m_translations(0),
  m_categories(false) {

  QHash<int, QByteArray> roles;
  roles[IdRole] = "translationId";
  roles[NameRole] = "name";

  setRoleNames(roles);
}

TranslationModel::~TranslationModel() {

}

int TranslationModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_ids.size();
  }

  return 0;
}

QVariant TranslationModel::data(const QModelIndex& index, int role) const {
  if ((role == NameRole || role == IdRole) && index.row() < m_ids.size()) {
    if (role == IdRole) {
      return m_ids[index.row()];
    }
    else if (role == NameRole) {
      return m_categories ? m_translations->categoryName(m_ids[index.row()]) :
	m_translations->translationName(m_ids[index.row()]);
    }
  }

  return QVariant();

}

Translations *TranslationModel::translations() const {
  return m_translations;
}

void TranslationModel::setTranslations(Translations *translations) {
  if (m_translations != translations) {

    m_translations = translations;

    emit translationsChanged();

    translationsUpdated();
  }
}

void TranslationModel::addId(int id) {
  if (m_ids.indexOf(id) != -1) {
    qCritical() << "id already known" << id;
    return;
  }

  beginInsertRows(QModelIndex(), m_ids.size(), m_ids.size());
  m_ids << id;
  endInsertRows();
}

void TranslationModel::removeId(int id) {
  int index = m_ids.indexOf(id);
  if (index == -1) {
    qCritical() << "unknown id" << id;
    return;
  }

  beginRemoveRows(QModelIndex(), index, index);
  m_ids.removeAt(index);
  endRemoveRows();
}

void TranslationModel::setIds(const QList<int>& ids) {
  if (!m_ids.isEmpty()) {
    beginRemoveRows(QModelIndex(), 0, m_ids.size() - 1);
    m_ids.clear();
    endRemoveRows();
  }

  if (!ids.isEmpty()) {
    beginInsertRows(QModelIndex(), 0, ids.size() - 1);
    m_ids = ids;
    endInsertRows();
  }
}

InstalledTranslationsModel::InstalledTranslationsModel(QObject *parent) :
  TranslationModel(parent) {

}

InstalledTranslationsModel::~InstalledTranslationsModel() {

}

void InstalledTranslationsModel::translationsUpdated() {
  QObject::connect(m_translations, SIGNAL(added(int)), this, SLOT(addId(int)));
  QObject::connect(m_translations, SIGNAL(removed(int)), this, SLOT(removeId(int)));
  QObject::connect(m_translations, SIGNAL(refreshed()), this, SLOT(refresh()));

  refresh();
}

void InstalledTranslationsModel::refresh() {
  setIds(m_translations->installed());
}

ActiveTranslationsModel::ActiveTranslationsModel(QObject *parent) :
  TranslationModel(parent) {

}

ActiveTranslationsModel::~ActiveTranslationsModel() {

}

void ActiveTranslationsModel::translationsUpdated() {
  QObject::connect(m_translations, SIGNAL(activeChanged()), this, SLOT(activeChanged()));

  activeChanged();
}

void ActiveTranslationsModel::activeChanged() {
  setIds(m_translations->active());
}

TranslationCategoriesModel::TranslationCategoriesModel(QObject *parent) :
  TranslationModel(parent) {

  m_categories = true;
}

TranslationCategoriesModel::~TranslationCategoriesModel() {

}

void TranslationCategoriesModel::translationsUpdated() {
  setIds(m_translations->categories());
}

TranslationCollection::TranslationCollection(QObject *parent) :
  TranslationModel(parent),
  m_language(-1) {

}

TranslationCollection::~TranslationCollection() {

}

int TranslationCollection::language() const {
  return m_language;
}

void TranslationCollection::setLanguage(int language) {
  if (language != m_language) {
    m_language = language;

    translationsUpdated();

    emit languageChanged();
  }
}

void TranslationCollection::translationsUpdated() {
  if (m_translations && m_language != -1) {
    setIds(m_translations->translations(m_language));
  }
}
