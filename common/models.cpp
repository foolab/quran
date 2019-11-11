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

#include "models.h"
#include "translations.h"
#include "translation.h"
#include "recitations.h"
#include "recitation.h"
#include <QQmlEngine>

typedef enum {
  TranslationRole = Qt::UserRole,
  RecitationRole = Qt::UserRole + 1,
  SectionRole = Qt::UserRole + 2,
  VisibleRole = Qt::UserRole + 3,
} Roles;

SortModel::SortModel(QObject *parent) :
  QSortFilterProxyModel(parent) {

  setFilterKeyColumn(0);
  setDynamicSortFilter(true);
}

SortModel::~SortModel() {

}

QAbstractItemModel *SortModel::model() {
  return sourceModel();
}

void SortModel::setModel(QAbstractItemModel *model) {
  if (sourceModel() != model) {
    setSourceModel(model);
    emit modelChanged();

    sort(0);
  }
}

bool SortModel::lessThan(const QModelIndex& source_left,
				     const QModelIndex& source_right) const {
  return dynamic_cast<LessThanInterface *>(sourceModel())->lessThan(source_left, source_right);
}

TranslationsModel::TranslationsModel(QObject *parent) :
  QAbstractListModel(parent),
  m_translations(0) {

}

TranslationsModel::~TranslationsModel() {

}

bool TranslationsModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) {
  bool visible_left = data(source_left, VisibleRole).toBool();
  bool visible_right = data(source_right, VisibleRole).toBool();

  if (visible_left != visible_right) {
    return visible_left;
  }

  QString sec_left = data(source_left, SectionRole).toString();
  QString sec_right = data(source_right, SectionRole).toString();

  int n = QString::localeAwareCompare(sec_left, sec_right);
  if (n != 0) {
    return n < 0;
  }

  Translation *left = m_translations->translation(source_left.row());
  Translation *right = m_translations->translation(source_right.row());

  n = QString::localeAwareCompare(left->name(), right->name());
  if (n != 0) {
    return n < 0;
  }

  // What can we do?!
  return true;
}

int TranslationsModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid() && m_translations) {
    return m_translations->count();
  }

  return 0;
}

QVariant TranslationsModel::data(const QModelIndex& index, int role) const {
  Translation *t;

  if (m_translations && index.row() >= 0 && index.row() < m_translations->count()) {
    switch (role) {
    case SectionRole:
      return m_translations->translation(index.row())->status() == Translation::Installed ?
	tr("Downloaded") : m_translations->translation(index.row())->language();

    case TranslationRole:
      t = m_translations->translation(index.row());
      QQmlEngine::setObjectOwnership(t, QQmlEngine::CppOwnership);
      return QVariant::fromValue<QObject *>(t);

    case VisibleRole:
      return m_translations->translation(index.row())->status() == Translation::Installed;

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
      emit dataChanged(idx1, idx1);
    }
  }
}

RecitationsModel::RecitationsModel(QObject *parent) :
  QAbstractListModel(parent),
  m_recitations(0) {

}

RecitationsModel::~RecitationsModel() {

}

bool RecitationsModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) {
  bool visible_left = data(source_left, VisibleRole).toBool();
  bool visible_right = data(source_right, VisibleRole).toBool();

  if (visible_left != visible_right) {
    return visible_left;
  }

  QString sec_left = data(source_left, SectionRole).toString();
  QString sec_right = data(source_right, SectionRole).toString();

  int n = QString::localeAwareCompare(sec_left, sec_right);
  if (n != 0) {
    return n < 0;
  }

  Recitation *left = m_recitations->recitation(source_left.row());
  Recitation *right = m_recitations->recitation(source_right.row());

  n = QString::localeAwareCompare(left->name(), right->name());
  if (n != 0) {
    return n < 0;
  }

  n = QString::localeAwareCompare(left->quality(), right->quality());
  if (n != 0) {
    return n < 0;
  }

  // What can we do?!
  return true;
}

int RecitationsModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid() && m_recitations) {
    return m_recitations->count();
  }

  return 0;
}

QVariant RecitationsModel::data(const QModelIndex& index, int role) const {
  Recitation *r;

  if (m_recitations && index.row() >= 0 && index.row() < m_recitations->count()) {
    switch (role) {
    case SectionRole:
      return m_recitations->recitation(index.row())->status() == Recitation::Installed ?
	tr("Enabled") : tr("Available");

    case RecitationRole:
      r = m_recitations->recitation(index.row());
      QQmlEngine::setObjectOwnership(r, QQmlEngine::CppOwnership);
      return QVariant::fromValue<QObject *>(r);

    case VisibleRole:
      return m_recitations->recitation(index.row())->status() == Recitation::Installed;

    default:
      break;
    }
  }

  return QVariant();
}

Recitations *RecitationsModel::source() const {
  return m_recitations;
}

void RecitationsModel::setSource(Recitations *recitations) {
  if (m_recitations != recitations) {

    if (m_recitations) {
      QObject::disconnect(m_recitations, SIGNAL(refreshed()), this, SLOT(refresh()));
      QObject::disconnect(m_recitations, SIGNAL(recitationEnabled(Recitation *)),
			  this, SLOT(recitationChanged(Recitation *)));
      QObject::disconnect(m_recitations, SIGNAL(recitationDisabled(Recitation *)()),
			  this, SLOT(recitationChanged(Recitation *)));
    }

    m_recitations = recitations;

    if (m_recitations) {
      QObject::connect(m_recitations, SIGNAL(refreshed()), this, SLOT(refresh()));
      QObject::connect(m_recitations, SIGNAL(recitationEnabled(Recitation *)),
		       this, SLOT(recitationChanged(Recitation *)));
      QObject::connect(m_recitations, SIGNAL(recitationDisabled(Recitation *)),
		       this, SLOT(recitationChanged(Recitation *)));
    }

    emit sourceChanged();

    refresh();
  }
}

void RecitationsModel::refresh() {
  beginResetModel();
  // Ok :(
  endResetModel();
}

QHash<int, QByteArray> RecitationsModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[RecitationRole] = "recitation";
  roles[SectionRole] = "section";
  roles[VisibleRole] = "visible";

  return roles;
}

void RecitationsModel::recitationChanged(Recitation *t) {
  for (int x = 0; x < m_recitations->count(); x++) {
    if (m_recitations->recitation(x) == t) {
      QModelIndex idx1 = index(x, 0);
      emit dataChanged(idx1, idx1);
    }
  }
}
