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

#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

class Settings;
class DataProvider;
class NumberFormatter;
class QLabel;

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  SettingsDialog(Settings *settings, DataProvider *data, NumberFormatter *formatter,
		 QWidget *parent = 0);
  ~SettingsDialog();

private slots:
  void populatePreview();
  void changeTextType(int text);

private:
  Settings *m_settings;
  DataProvider *m_data;
  NumberFormatter *m_formatter;

  QLabel *m_preview;
};

#endif /* SETTINGS_DIALOG_H */
