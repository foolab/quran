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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtDeclarative>

class QUrl;
class QApplication;
class QDeclarativeView;
class IconProvider;
class QLatin1String;
class QmlFileEngineHandler;

class Application {
public:
  Application(int& argc, char **argv);
  ~Application();

  void registerQmlTypes();
  void addImageProvider(const QLatin1String& id, IconProvider *provider);
  bool load(const QUrl& url);
  int exec();

private:
  QApplication *m_app;
  QDeclarativeView *m_view;
  QmlFileEngineHandler *m_handler;
};

#endif /* APPLICATION_H */
