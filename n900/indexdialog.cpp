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

#include "indexdialog.h"
#include <QTreeWidget>
#include "dataprovider.h"
#include "numberformatter.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include "settings.h"

// TODO: select the current sura.
IndexDialog::IndexDialog(Settings *settings, DataProvider *data, NumberFormatter *formatter,
			 QWidget *parent) : QDialog(parent), m_settings(settings),
					    m_data(data), m_formatter(formatter), m_sura(-1) {

  setWindowTitle(tr("Index"));
  setAttribute(Qt::WA_Maemo5PortraitOrientation);

  setMinimumHeight(1000); // Something big

  m_widget = new QTreeWidget(this);
  m_widget->setLayoutDirection(Qt::RightToLeft);
  m_widget->setColumnCount(2);
  m_widget->setHeaderHidden(true);
  m_widget->setColumnWidth(0, 200);

  for (int x = 0; x < m_data->suraCount(); x++) {
    QTreeWidgetItem *item = new QTreeWidgetItem(m_widget);
    item->setText(0, m_formatter->number(x + 1));
    item->setText(1, m_data->fullSuraName(x));
    item->setData(0, Qt::UserRole, x);
  }

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_widget);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
  buttonBox->addButton(tr("Go"), QDialogButtonBox::AcceptRole);

  layout->addWidget(buttonBox);

  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(doAccept()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

IndexDialog::~IndexDialog() {

}

void IndexDialog::doAccept() {
  QList<QTreeWidgetItem *> items = m_widget->selectedItems();
  if (!items.isEmpty()) {
    QTreeWidgetItem *item = items.at(0);
    m_sura = item->data(0, Qt::UserRole).toInt();
  }

  accept();
}

int IndexDialog::sura() const {
  return m_sura;
}
