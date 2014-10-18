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

#include "application.h"
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "iconprovider.h"

Application::Application(int& argc, char **argv) {
  m_app = new QGuiApplication(argc, argv);

  m_app->setApplicationName("harbour-quran");
  m_app->setApplicationDisplayName(QObject::tr("Holy Quran"));
  m_engine = new QQmlApplicationEngine;
}

Application::~Application() {
  delete m_engine;
  delete m_app;
}

void Application::addImageProvider(const QLatin1String& id, IconProvider *provider) {
  m_engine->addImageProvider(id, provider);
}

bool Application::load(const QUrl& url) {
  QObject::connect(m_engine, SIGNAL(quit()), m_app, SLOT(quit()));

  m_engine->load(url);

  return true;
}

int Application::exec() {
  return m_app->exec();
}
