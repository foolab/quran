#include "windowcontroller.h"
#include "settings.h"
#include <QWidget>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <QDeclarativeItem>

WindowController::WindowController(QWidget *view, Settings *settings, QDeclarativeItem *root,
				   QObject *parent)
  : QObject(parent), m_view(view), m_settings(settings), m_root(root) {

  QObject::connect(m_settings, SIGNAL(fullScreenChanged()), this, SLOT(show()));

  QObject::connect(m_settings, SIGNAL(orientationChanged()), this, SLOT(setOrientation()));

  QObject::connect(MApplication::activeApplicationWindow()->currentPage(),
		   SIGNAL(exposedContentRectChanged()),
		   this, SLOT(exposedContentRectChanged()));
}

WindowController::~WindowController() {
  m_view = 0;
  m_settings = 0;
}

void WindowController::show() {
  if (m_settings->fullScreen()) {
    MApplication::activeApplicationWindow()->showFullScreen();
  }
  else {
    MApplication::activeApplicationWindow()->showNormal();
  }
}

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
