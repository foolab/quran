/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
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

#include "favoritesdialog.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include "bookmarks.h"
#include <QTreeWidget>
#include "dataprovider.h"
#include "settings.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>
#include <QMaemo5InformationBox>

// TODO: get rid of the serailze/deserialize we are doing.
FavoritesDialog::FavoritesDialog(Settings *settings, Bookmarks *bookmarks, DataProvider *data,
				 QWidget *parent) :
  QDialog(parent), m_bookmarks(bookmarks), m_sura(-1), m_aya(-1) {

  setAttribute(Qt::WA_Maemo5PortraitOrientation);
  setWindowTitle(tr("Favorites"));

  setMinimumHeight(1000); // Something big

  m_widget = new QTreeWidget(this);
  m_widget->setColumnCount(1);

  QList<QTreeWidgetItem *> items;

  QMap<int, QList<int> > map;

  QVariantList bs = m_bookmarks->bookmarks();
  foreach(const QVariant& var, bs) {
    uint b = var.value<uint>();
    map[m_bookmarks->sura(b)].append(m_bookmarks->aya(b));
  }

  for (QMap<int, QList<int> >::const_iterator iter = map.constBegin();
       iter != map.constEnd(); iter++) {
    int sura = iter.key();
    QList<int> ayat = iter.value();
    qSort(ayat);

    if (ayat.isEmpty()) {
      continue;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem;
    QFont f = item->font(0);
    f.setFamily(settings->fontFamily());
    item->setFont(0, f);

    item->setText(0, data->fullSuraName(sura));
    items << item;

    foreach(int aya, ayat) {
      QTreeWidgetItem *item2 = new QTreeWidgetItem(item);
      item2->setText(0, data->text(sura, aya));
      item2->setFont(0, f);
      item2->setData(0, Qt::UserRole, m_bookmarks->serialize(sura, aya));
    }
  }

  m_widget->insertTopLevelItems(0, items);

  foreach (QTreeWidgetItem *item, items) {
    m_widget->expandItem(item);
  }

  m_widget->setHeaderHidden(true);
  m_widget->setLayoutDirection(Qt::RightToLeft);
  m_widget->setRootIsDecorated(false);
  m_widget->setExpandsOnDoubleClick(false);
  m_widget->setTextElideMode(Qt::ElideRight); // TODO:

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_widget);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
  QObject::connect(buttonBox->addButton(tr("Go"), QDialogButtonBox::ActionRole),
		   SIGNAL(clicked()), this, SLOT(go()));

  QObject::connect(buttonBox->addButton(tr("Clear"), QDialogButtonBox::ActionRole),
		   SIGNAL(clicked()), this, SLOT(clear()));

  QObject::connect(buttonBox->addButton(tr("Remove"), QDialogButtonBox::ActionRole),
		   SIGNAL(clicked()), this, SLOT(remove()));

  layout->addWidget(buttonBox);

  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

FavoritesDialog::~FavoritesDialog() {

}

void FavoritesDialog::go() {
  QList<QTreeWidgetItem *> items = m_widget->selectedItems();

  if (items.isEmpty() || items.at(0)->childCount() != 0) {
    QMaemo5InformationBox::information(this, tr("Please select a verse"));
    return;
  }

  QTreeWidgetItem *item = items.at(0);

  uint b = item->data(0, Qt::UserRole).value<uint>();

  m_sura = m_bookmarks->sura(b);
  m_aya = m_bookmarks->aya(b);

  accept();
}

void FavoritesDialog::clear() {
  if (QMessageBox::question(this, tr("Favorites"), tr("Clear all favorites?"),
			    QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    m_bookmarks->clear();
    reject();
  }
}

void FavoritesDialog::remove() {
  QList<QTreeWidgetItem *> items = m_widget->selectedItems();

  if (items.isEmpty() || items.at(0)->childCount() != 0) {
    QMaemo5InformationBox::information(this, tr("Please select a verse"));
    return;
  }

  QTreeWidgetItem *item = items.at(0);

  if (QMessageBox::question(this, tr("Favorites"), tr("Remove favorite?"),
			    QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {

    uint b = item->data(0, Qt::UserRole).value<uint>();
    m_bookmarks->remove(b);
    QTreeWidgetItem *parent = item->parent();
    parent->removeChild(item);
    delete item;
    if (parent->childCount() == 0) {
      int idx = m_widget->indexOfTopLevelItem(parent);
      QTreeWidgetItem *above = m_widget->itemAbove(parent);
      delete m_widget->takeTopLevelItem(idx);

      if (above) {
	m_widget->setCurrentItem(above);
      }
    }

    if (m_widget->topLevelItemCount() == 0) {
      reject();
    }
  }
}

int FavoritesDialog::sura() const {
  return m_sura;
}

int FavoritesDialog::aya() const {
  return m_aya;
}
