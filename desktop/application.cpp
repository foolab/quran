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

#include "application.h"
#include <QGuiApplication>
#include <QQuickView>
#include "mockandroidsupport.h"

Application::Application(int& argc, char **argv) {
  setApplication(new QGuiApplication(argc, argv), "quran");
  setView(new QQuickView);
}

Application::~Application() {

}

void Application::registerQmlTypes() {
  qmlRegisterType<AndroidSupport>("QuranAndroid", 1, 0, "AndroidSupport");
}
