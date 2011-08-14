#ifndef WINDOW_CONTROLLER_H
#define WINDOW_CONTROLLER_H

#include <QObject>

class QDeclarativeView;
class Settings;

class WindowController : public QObject {
  Q_OBJECT

public:
  WindowController(QDeclarativeView *view, Settings *settings, QObject *parent = 0);
  ~WindowController();

public slots:
  void show();

#ifdef Q_WS_MAEMO_5
  void setOrientation();
#endif

private:
  QDeclarativeView *m_view;
  Settings *m_settings;
};

#endif /* WINDOW_CONTROLLER_H */
