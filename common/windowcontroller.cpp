/*
 * Copyright (c) 2011-2013 Mohammed Sameer <msameer@foolab.org>.
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

#include "windowcontroller.h"
#include "settings.h"
#include <QWidget>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <QDeclarativeItem>

WindowController::WindowController(QObject *parent)
  : QObject(parent), m_fullScreen(false), m_orientation(2) {

  QObject::connect(MApplication::activeApplicationWindow()->currentPage(),
		   SIGNAL(exposedContentRectChanged()),
		   this, SLOT(exposedContentRectChanged()));
}

WindowController::~WindowController() {

}

void WindowController::show() {
  if (m_fullScreen) {
    MApplication::activeApplicationWindow()->showFullScreen();
  }
  else {
    MApplication::activeApplicationWindow()->showNormal();
  }
}

void WindowController::applyOrientation() {
  MApplicationWindow *win = MApplication::activeApplicationWindow();
  switch (m_orientation) {
  case 0: // Auto
    win->setOrientationLocked(false);
    return;

  case 1: // Portrait
    win->setPortraitOrientation();
    win->setOrientationLocked(true);
    return;

  case 2: // Landscape
    win->setLandscapeOrientation();
    win->setOrientationLocked(true);
    return;

  default: // Heck!
    return;
  }
}

void WindowController::exposedContentRectChanged() {
  m_rect = MApplication::activeApplicationWindow()->currentPage()->exposedContentRect();
  emit widthChanged();
  emit heightChanged();
}

int WindowController::width() const {
  return m_rect.width();
}

int WindowController::height() const {
  return m_rect.height();
}

int WindowController::orientation() const {
  return m_orientation;
}

void WindowController::setOrientation(int orientation) {
  if (m_orientation != orientation) {
    m_orientation = orientation;
    applyOrientation();
    emit orientationChanged();
  }
}

bool WindowController::fullScreen() const {
  return m_fullScreen;
}

void WindowController::setFullScreen(bool fullScreen) {
  if (m_fullScreen != fullScreen) {
    m_fullScreen = fullScreen;
    show();
    emit fullScreenChanged();
  }
}
