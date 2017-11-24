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

#include "models.h"
#include "translations.h"
#include <QQmlEngine>
#include "translation.h"
#include <QDebug>

InstalledFilterModel::InstalledFilterModel(QObject *parent) :
  QSortFilterProxyModel(parent),
  m_value(-1),
  m_role(-1) {

  setFilterKeyColumn(0);
  setDynamicSortFilter(true);
}

InstalledFilterModel::~InstalledFilterModel() {

}

void InstalledFilterModel::init(QAbstractItemModel *model, const QString& installedPropertyName,
				int installedPropertyValue, const QString& role) {
  m_name = installedPropertyName;
  m_value = installedPropertyValue;


  QHash<int, QByteArray> roles = model->roleNames();
  foreach (int r, roles.keys()) {
    if (roles[r] == role) {
      m_role = r;
      break;
    }
  }

  setSourceModel(model);
}

bool InstalledFilterModel::filterAcceptsRow(int source_row,
					    const QModelIndex& source_parent) const {
  if (!sourceModel()) {
    return false;
  }

  QObject *object =
    sourceModel()->data(sourceModel()->index(source_row, 0, source_parent),
			m_role).value<QObject *>();
  if (!object) {
    return false;
  }

  return object->property(m_name.toUtf8()).toInt() == m_value;
}

VisibilityFilterModel::VisibilityFilterModel(QObject *parent) :
  QSortFilterProxyModel(parent) {

  setFilterKeyColumn(0);
  setDynamicSortFilter(true);
}

VisibilityFilterModel::~VisibilityFilterModel() {

}

QAbstractItemModel *VisibilityFilterModel::model() {
  return sourceModel();
}

void VisibilityFilterModel::setModel(QAbstractItemModel *model) {
  if (sourceModel() != model) {
    setSourceModel(model);
    emit modelChanged();
  }
}

bool VisibilityFilterModel::filterAcceptsRow(int source_row,
					     const QModelIndex& source_parent) const {
  return sourceModel()->data(sourceModel()->index(source_row, 0, source_parent),
			     TranslationsModel::VisibleRole).toBool();
}

// For the sake of simplicity we will implement the model via a hack:
// Duplicate all indexes
// Show only installed indexes for the first half
// Show only non installed indexes for the second half.
// This needs cooperation from the view though or a filter model on top.
TranslationsModel::TranslationsModel(QObject *parent) :
  QAbstractListModel(parent),
  m_translations(0) {

}

TranslationsModel::~TranslationsModel() {

}

int TranslationsModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid() && m_translations) {
    return m_translations->count() * 2;
  }

  return 0;
}

QVariant TranslationsModel::data(const QModelIndex& index, int role) const {
  if (m_translations && index.row() >= 0 && index.row() < m_translations->count() * 2) {
    switch (role) {
    case SectionRole:
      return index.row() < m_translations->count() ? tr("Downloaded") : m_translations->translation(index.row() - m_translations->count())->language();

    case TranslationRole:
      if (index.row() < m_translations->count()) {
	Translation *t = m_translations->translation(index.row());
	QQmlEngine::setObjectOwnership(t, QQmlEngine::CppOwnership);
	return QVariant::fromValue<QObject *>(t);
      } else {
	Translation *t = m_translations->translation(index.row() - m_translations->count());
	QQmlEngine::setObjectOwnership(t, QQmlEngine::CppOwnership);
	return QVariant::fromValue<QObject *>(t);
      }

    case VisibleRole:
      if (index.row() < m_translations->count()) {
	return m_translations->translation(index.row())->status() == Translation::Installed;
      } else {
	return m_translations->translation(index.row() - m_translations->count())->status() != Translation::Installed;
      }
    default:
      break;
    }
  }

  return QVariant();
}

Translations *TranslationsModel::source() const {
  return m_translations;
}

void TranslationsModel::setSource(Translations *translations) {
  if (m_translations != translations) {

    if (m_translations) {
      QObject::disconnect(m_translations, SIGNAL(refreshed()), this, SLOT(refresh()));
      QObject::disconnect(m_translations, SIGNAL(translationInstalled(Translation *)),
			  this, SLOT(translationChanged(Translation *)));
      QObject::disconnect(m_translations, SIGNAL(translationRemoved(Translation *)()),
			  this, SLOT(translationChanged(Translation *)));
    }

    m_translations = translations;

    if (m_translations) {
      QObject::connect(m_translations, SIGNAL(refreshed()), this, SLOT(refresh()));
      QObject::connect(m_translations, SIGNAL(translationInstalled(Translation *)),
		       this, SLOT(translationChanged(Translation *)));
      QObject::connect(m_translations, SIGNAL(translationRemoved(Translation *)),
		       this, SLOT(translationChanged(Translation *)));
    }

    emit sourceChanged();

    refresh();
  }
}

void TranslationsModel::refresh() {
  beginResetModel();
  // Ok :(
  endResetModel();
}

QHash<int, QByteArray> TranslationsModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TranslationRole] = "translation";
  roles[SectionRole] = "section";
  roles[VisibleRole] = "visible";

  return roles;
}

void TranslationsModel::translationChanged(Translation *t) {
  for (int x = 0; x < m_translations->count(); x++) {
    if (m_translations->translation(x) == t) {
      QModelIndex idx1 = index(x, 0);
      QModelIndex idx2 = index(x + m_translations->count(), 0);
      emit dataChanged(idx1, idx1);
      emit dataChanged(idx2, idx2);
    }
  }
}
