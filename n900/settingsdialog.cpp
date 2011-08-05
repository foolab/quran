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

#include "settingsdialog.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "settings.h"
#include "dataprovider.h"
#include "numberformatter.h"
#include <QGroupBox>
#include <QSlider>
#include <QPushButton>
#include <QButtonGroup>
#include <QMaemo5InformationBox>

SettingsDialog::SettingsDialog(Settings *settings, DataProvider *data, NumberFormatter *formatter,
			       QWidget *parent) : QDialog(parent), m_settings(settings),
						  m_data(data), m_formatter(formatter) {

  setAttribute(Qt::WA_Maemo5PortraitOrientation);
  setWindowTitle(tr("Settings"));

  setMinimumHeight(1000); // Something big

  QWidget *widget = new QWidget(this);
  QVBoxLayout *l = new QVBoxLayout(widget);

  // Preview...
  m_preview = new QLabel(this);
  m_preview->setAlignment(Qt::AlignCenter);
  m_preview->setWordWrap(true);
  l->addWidget(m_preview, Qt::AlignCenter);

  // Text Size...
  {
    QSlider *slider = new QSlider(this);
    slider->setRange(m_settings->minFontSize(), m_settings->maxFontSize());
    slider->setValue(m_settings->fontSize());
    QGroupBox *box = new QGroupBox(tr("Font size:"), this);
    QVBoxLayout *layout = new QVBoxLayout(box);
    layout->addWidget(slider);
    slider->setOrientation(Qt::Horizontal);
    l->addWidget(box);
    QObject::connect(slider, SIGNAL(valueChanged(int)), m_settings, SLOT(setFontSize(int)));
  }

  // Text type:
  {
    QGroupBox *box = new QGroupBox(tr("Text type:"), this);
    QHBoxLayout *layout = new QHBoxLayout(box);

    QPushButton *b1 = new QPushButton(tr("Uthmani"), this);
    b1->setCheckable(true);

    QPushButton *b2 = new QPushButton(tr("Simple"), this);
    b2->setCheckable(true);

    QButtonGroup *g = new QButtonGroup(this);
    g->setExclusive(true);
    g->addButton(b1, 0);
    g->addButton(b2, 1);

    layout->addWidget(b1);
    layout->addWidget(b2);

    l->addWidget(box);

    g->button(m_settings->textType())->setChecked(true);

    QObject::connect(g, SIGNAL(buttonClicked(int)), this, SLOT(changeTextType(int)));
  }

  // Number format:
  {
    QGroupBox *box = new QGroupBox(tr("Number format:"), this);
    QHBoxLayout *layout = new QHBoxLayout(box);

    QPushButton *b1 = new QPushButton(tr("Hindi"), this);
    b1->setCheckable(true);

    QPushButton *b2 = new QPushButton(tr("Arabic"), this);
    b2->setCheckable(true);

    QButtonGroup *g = new QButtonGroup(this);
    g->setExclusive(true);
    g->addButton(b1, 0);
    g->addButton(b2, 1);

    layout->addWidget(b1);
    layout->addWidget(b2);

    l->addWidget(box);

    g->button(m_settings->numberFormat())->setChecked(true);

    QObject::connect(g, SIGNAL(buttonClicked(int)), m_settings, SLOT(setNumberFormat(int)));
  }

  populatePreview();

  QScrollArea *area = new QScrollArea(this);
  area->setWidget(widget);
  area->setWidgetResizable(true);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(area);

  QObject::connect(m_settings, SIGNAL(fontSizeChanged()), this, SLOT(populatePreview()));
  QObject::connect(m_settings, SIGNAL(textTypeChanged()), this, SLOT(populatePreview()));
  QObject::connect(m_settings, SIGNAL(numberFormatChanged()), this, SLOT(populatePreview()));
}

SettingsDialog::~SettingsDialog() {

}

void SettingsDialog::populatePreview() {
  // HACK: Seems the only way to force the direction is to do this.
  m_preview->setText("<div dir='rtl'>" +
		     m_data->text(0, 0) + " (" + m_formatter->number(1) + ")" + "</div>");
  m_preview->setFont(QFont(m_settings->fontFamily(), m_settings->fontSize()));
}

void SettingsDialog::changeTextType(int text) {
  if (!m_data->setText(text)) {
    QMaemo5InformationBox::information(this, tr("Failed to load the text"));
  }
  else {
    m_settings->setTextType(text);
  }
}
