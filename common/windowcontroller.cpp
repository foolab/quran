#include "windowcontroller.h"
#include "settings.h"
#include <QDeclarativeView>

WindowController::WindowController(QDeclarativeView *view, Settings *settings, QObject *parent)
  : QObject(parent), m_view(view), m_settings(settings) {

  QObject::connect(m_settings, SIGNAL(fullScreenChanged()), this, SLOT(show()));

#ifdef Q_WS_MAEMO_5
  QObject::connect(m_settings, SIGNAL(orientationChanged()), this, SLOT(setOrientation()));
#endif
}

WindowController::~WindowController() {
  m_view = 0;
  m_settings = 0;
}

void WindowController::show() {
  if (m_settings->fullScreen()) {
    m_view->showFullScreen();
  }
  else {
    m_view->showMaximized();
  }
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
#endif
