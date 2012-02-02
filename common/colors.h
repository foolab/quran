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

#define ADD_FUNCTION(name, day, night)      \
QColor name() const {                       \
  return value(#name, day, night);          \
}

class Colors : public QObject {
  Q_OBJECT

  Q_PROPERTY(QColor textColor READ textColor NOTIFY colorsChanged);
  ADD_FUNCTION(textColor, Qt::black, Qt::white);

  Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY colorsChanged);
  ADD_FUNCTION(backgroundColor, Qt::white, Qt::black);

  Q_PROPERTY(QColor verseColor READ verseColor NOTIFY colorsChanged);
  ADD_FUNCTION(verseColor, Qt::black, Qt::white);

  Q_PROPERTY(QColor titleColor READ titleColor NOTIFY colorsChanged);
  ADD_FUNCTION(titleColor, Qt::white, Qt::black);

  Q_PROPERTY(QColor subtitleColor READ subtitleColor NOTIFY colorsChanged);
  ADD_FUNCTION(subtitleColor, Qt::white, Qt::black);

  Q_PROPERTY(QColor highlightColor READ highlightColor NOTIFY colorsChanged);
  ADD_FUNCTION(highlightColor, Qt::red, Qt::red);

  Q_PROPERTY(QColor faderColor READ faderColor NOTIFY colorsChanged);
  ADD_FUNCTION(faderColor, "steelblue", Qt::black);

  Q_PROPERTY(QColor sectionColor READ sectionColor NOTIFY colorsChanged);
  ADD_FUNCTION(sectionColor, QColor(163, 218, 244), QColor(163, 218, 244));

  Q_PROPERTY(QColor pageTitleTextColor READ pageTitleTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(pageTitleTextColor, Qt::white, Qt::white);

  Q_PROPERTY(QColor pageTitleBackgroundColor READ pageTitleBackgroundColor NOTIFY colorsChanged);
  ADD_FUNCTION(pageTitleBackgroundColor, "steelblue", "steelblue");

  Q_PROPERTY(QColor dialogTitleTextColor READ dialogTitleTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(dialogTitleTextColor, Qt::white, Qt::white);

  Q_PROPERTY(QColor dialogTitleBackgroundColor READ dialogTitleBackgroundColor NOTIFY colorsChanged);
  ADD_FUNCTION(dialogTitleBackgroundColor, "steelblue", "steelblue");

  Q_PROPERTY(QColor dialogTextColor READ dialogTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(dialogTextColor, Qt::black, Qt::white);

  Q_PROPERTY(QColor buttonBackgroundColor READ buttonBackgroundColor NOTIFY colorsChanged);
  ADD_FUNCTION(buttonBackgroundColor, "steelblue", "steelblue");

  Q_PROPERTY(QColor buttonTextColor READ buttonTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(buttonBorderColor, Qt::black, Qt::white);

  Q_PROPERTY(QColor buttonBorderColor READ buttonBorderColor NOTIFY colorsChanged);
  ADD_FUNCTION(buttonTextColor, Qt::black, Qt::white);

  Q_PROPERTY(QColor infoBannerTextColor READ infoBannerTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(infoBannerTextColor, Qt::white, Qt::white);

  Q_PROPERTY(QColor infoBannerBackgroundColor READ infoBannerBackgroundColor NOTIFY colorsChanged);
  ADD_FUNCTION(infoBannerBackgroundColor, "steelblue", "steelblue");

  Q_PROPERTY(QColor infoBannerBorderColor READ infoBannerBorderColor NOTIFY colorsChanged);
  ADD_FUNCTION(infoBannerBorderColor, Qt::black, Qt::black);

  Q_PROPERTY(QColor pressedColor READ pressedColor NOTIFY colorsChanged);
  ADD_FUNCTION(pressedColor, "lightsteelblue", "lightsteelblue");

  Q_PROPERTY(QColor pressedTextColor READ pressedTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(pressedTextColor, "white", "white");

  Q_PROPERTY(QColor errorColor READ errorColor NOTIFY colorsChanged);
  ADD_FUNCTION(errorColor, "red", "red");

  Q_PROPERTY(QColor selectionTextColor READ selectionTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(selectionTextColor, "white", "white");

  Q_PROPERTY(QColor selectionBackgroundColor READ selectionBackgroundColor NOTIFY colorsChanged);
  ADD_FUNCTION(selectionBackgroundColor, "steelblue", "steelblue");

  Q_PROPERTY(QColor textFieldColor READ textFieldColor NOTIFY colorsChanged);
  ADD_FUNCTION(textFieldColor, "lightsteelblue", "lightsteelblue");

  Q_PROPERTY(QColor textFieldBorderColor READ textFieldBorderColor NOTIFY colorsChanged);
  ADD_FUNCTION(textFieldBorderColor, "steelblue", "steelblue");

  Q_PROPERTY(QColor textFieldTextColor READ textFieldTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(textFieldTextColor, Qt::black, Qt::black);

  Q_PROPERTY(QColor textFieldPlaceholderTextColor READ textFieldPlaceholderTextColor NOTIFY colorsChanged);
  ADD_FUNCTION(textFieldPlaceholderTextColor, "lightsteelblue", "lightsteelblue");

public:
  Colors(const QString& path, Settings *settings, QObject *parent = 0);
  ~Colors();

signals:
  void colorsChanged();

private slots:
  void themeChanged();
  void nightModeChanged();

private:
  QColor value(const QString& name, const QColor& day, const QColor& night) const;
  QColor value(const QString& section, const QColor& defaultColor) const;

  QSettings *m_ini;
  Settings *m_settings;
  const QString m_path;
  QString m_id;
  bool m_nightMode;
};

#endif /* COLORS_H */
