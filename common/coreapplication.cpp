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

#include "coreapplication.h"
#include "iconprovider.h"
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QDebug>

CoreApplication::CoreApplication() :
  m_app(nullptr),
  m_view(nullptr) {

}

CoreApplication::~CoreApplication() {
  delete m_view;
  delete m_app;
}

void CoreApplication::setApplication(QGuiApplication *app, const char *name) {
  m_app = app;
  m_app->setApplicationName(name);
  m_app->setApplicationDisplayName(QObject::tr("Holy Quran"));
}

void CoreApplication::setView(QQuickView *view) {
  m_view = view;
  m_view->setResizeMode(QQuickView::SizeRootObjectToView);
}

void CoreApplication::init() {
  m_view->setTitle(m_app->applicationDisplayName());
  QObject::connect(m_view->engine(), SIGNAL(quit()), m_app, SLOT(quit()));

  m_view->engine()->addImageProvider(QLatin1String("icon"), new IconProvider);

  registerQmlTypes();
}

bool CoreApplication::load(const QUrl& url) {
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

int CoreApplication::exec() {
  m_view->show();
  return m_app->exec();
}
