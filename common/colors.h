/*
 * Copyright (c) 2012 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef COLORS_H
#define COLORS_H

#include <QObject>
#include <QColor>

class QSettings;
class Settings;

class Colors : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString themeId READ themeId WRITE setThemeId NOTIFY colorsChanged);
  Q_PROPERTY(bool nightMode READ nightMode WRITE setNightMode NOTIFY colorsChanged);

  Q_PROPERTY(QColor textColor READ textColor NOTIFY colorsChanged);
  Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY colorsChanged);
  Q_PROPERTY(QColor verseColor READ verseColor NOTIFY colorsChanged);
  Q_PROPERTY(QColor titleColor READ titleColor NOTIFY colorsChanged);
  Q_PROPERTY(QColor subtitleColor READ subtitleColor NOTIFY colorsChanged);
  Q_PROPERTY(QColor highlightColor READ highlightColor NOTIFY colorsChanged);
  Q_PROPERTY(QColor faderColor READ faderColor NOTIFY colorsChanged);
  Q_PROPERTY(QColor sectionColor READ sectionColor NOTIFY colorsChanged);

public:
  Colors(const QString& path, Settings *settings, QObject *parent = 0);
  ~Colors();

  void setThemeId(const QString& id);
  QString themeId() const;

  void setNightMode(bool enabled);
  bool nightMode() const;

  QColor textColor() const;
  QColor backgroundColor() const;
  QColor verseColor() const;
  QColor titleColor() const;
  QColor subtitleColor() const;
  QColor highlightColor() const;
  QColor faderColor() const;
  QColor sectionColor() const;

signals:
  void colorsChanged();

private slots:
  void themeChanged();

private:
  QColor value(const QString& name, const QColor& day, const QColor& night) const;

  QSettings *m_ini;
  Settings *m_settings;
  const QString m_path;
  QString m_id;
  bool m_nightMode;
};

#endif /* COLORS_H */
