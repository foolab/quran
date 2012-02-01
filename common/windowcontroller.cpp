#include "windowcontroller.h"
#include "settings.h"
#include <QWidget>
#ifndef Q_WS_MAEMO_5
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <QDeclarativeItem>
#endif

WindowController::WindowController(QWidget *view, Settings *settings, QDeclarativeItem *root,
				   QObject *parent)
  : QObject(parent), m_view(view), m_settings(settings), m_root(root) {

  QObject::connect(m_settings, SIGNAL(fullScreenChanged()), this, SLOT(show()));

  QObject::connect(m_settings, SIGNAL(orientationChanged()), this, SLOT(setOrientation()));

#ifndef Q_WS_MAEMO_5
  QObject::connect(MApplication::activeApplicationWindow()->currentPage(),
		   SIGNAL(exposedContentRectChanged()),
		   this, SLOT(exposedContentRectChanged()));
#endif
}

WindowController::~WindowController() {
  m_view = 0;
  m_settings = 0;
}

void WindowController::show() {
#ifdef Q_WS_MAEMO_5
  if (m_settings->fullScreen()) {
    m_view->showFullScreen();
  }
  else {
    m_view->showMaximized();
  }
#else
  if (m_settings->fullScreen()) {
    MApplication::activeApplicationWindow()->showFullScreen();
  }
  else {
    MApplication::activeApplicationWindow()->showNormal();
  }
#endif
}

#ifdef Q_WS_MAEMO_5
void WindowController::setOrientation() {
  switch (m_settings->orientation()) {
  case 0: // Auto
    m_view->setAttribute(Qt::WA_Maemo5AutoOrientation);
    return;

  case 1: // Portrait
    m_view->setAttribute(Qt::WA_Maemo5PortraitOrientation);
    return;

  case 2: // Landscape
    m_view->setAttribute(Qt::WA_Maemo5LandscapeOrientation);
    return;

  default: // Heck!
    return;
  }
}
#else
void WindowController::setOrientation() {
  MApplicationWindow *win = MApplication::activeApplicationWindow();
  switch (m_settings->orientation()) {
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
  QRectF rect = MApplication::activeApplicationWindow()->currentPage()->exposedContentRect();
  m_root->setWidth(rect.width());
  m_root->setHeight(rect.height());
}
#endif
