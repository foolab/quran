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
#include <QQuickView>
#include <QQmlEngine>
#include <MDeclarativeCache>
#include "iconprovider.h"

Application::Application(int& argc, char **argv) {
  m_app = MDeclarativeCache::qApplication(argc, argv);

  m_app->setApplicationName("harbour-quran");
  m_app->setApplicationDisplayName(QObject::tr("Holy Quran"));

  m_view = MDeclarativeCache::qQuickView();

  m_view->setTitle(m_app->applicationDisplayName());
  m_view->setResizeMode(QQuickView::SizeRootObjectToView);
}

Application::~Application() {
  delete m_view;
  delete m_app;
}

void Application::addImageProvider(const QLatin1String& id, IconProvider *provider) {
  m_view->engine()->addImageProvider(id, provider);
}

bool Application::load(const QUrl& url) {
  QObject::connect(m_view->engine(), SIGNAL(quit()), m_app, SLOT(quit()));

  m_view->setSource(url);

  if (m_view->status() == QQuickView::Error) {
    qCritical() << "Errors loading QML:";
    QList<QQmlError> errors = m_view->errors();

    foreach (const QQmlError& error, errors) {
      qCritical() << error.toString();
    }

    return false;
  }

  return true;
}

int Application::exec() {
  m_view->showFullScreen();

  return m_app->exec();
}
