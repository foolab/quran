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

#include "aboutdialog.h"
#include "settings.h"
#include <QVBoxLayout>
#include <QLabel>
#include "legal.h"
#include <QScrollArea>
#include <QDialogButtonBox>

AboutDialog::AboutDialog(Settings *settings, QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("About"));
  setAttribute(Qt::WA_Maemo5PortraitOrientation);

  setMinimumHeight(1000); // Something big

  Legal legal;
  QWidget *widget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(widget);

  // Label
  {
    QLabel *label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setPixmap(QPixmap(DATA_DIR "/logo_160.png"));
    layout->addWidget(label, Qt::AlignCenter);
  }

  // Name and version
  {
    QLabel *label = new QLabel(tr("Holy Quran Version %1").arg(settings->version()), this);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    layout->addWidget(label, Qt::AlignCenter);
    QFont f = label->font();
    f.setBold(true);
    label->setFont(f);
  }

  // Copyright
  {
    QLabel *label = new QLabel(legal.copyright(), this);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setOpenExternalLinks(true);
    layout->addWidget(label, Qt::AlignCenter);
  }

  // App license:
  {
    QLabel *label = new QLabel(tr("Application license:"), this);
    QFont f = label->font();
    f.setBold(true);
    label->setFont(f);
    layout->addWidget(label);

    label = new QLabel(legal.appLicense(), this);
    label->setWordWrap(true);
    label->setOpenExternalLinks(true);
    layout->addWidget(label);
  }

  // Text license:
  {
    QLabel *label = new QLabel(tr("Text license:"), this);
    QFont f = label->font();
    f.setBold(true);
    label->setFont(f);
    layout->addWidget(label);

    label = new QLabel(legal.textLicense(), this);
    label->setWordWrap(true);
    label->setOpenExternalLinks(true);
    layout->addWidget(label);
  }

  // Font license:
  {
    QLabel *label = new QLabel(tr("Font license:"), this);
    QFont f = label->font();
    f.setBold(true);
    label->setFont(f);
    layout->addWidget(label);

    label = new QLabel(legal.fontLicense(), this);
    label->setWordWrap(true);
    label->setOpenExternalLinks(true);
    layout->addWidget(label);
  }

  QScrollArea *area = new QScrollArea(this);
  area->setWidget(widget);
  area->setWidgetResizable(true);

  QVBoxLayout *l = new QVBoxLayout(this);
  l->addWidget(area);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
						     | QDialogButtonBox::Cancel);

  l->addWidget(buttonBox);
  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

AboutDialog::~AboutDialog() {

}
