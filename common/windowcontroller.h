#ifndef WINDOW_CONTROLLER_H
#define WINDOW_CONTROLLER_H

#include <QObject>

class QWidget;
class Settings;
class QDeclarativeItem;

class WindowController : public QObject {
  Q_OBJECT

public:
  WindowController(QWidget *view, Settings *settings, QDeclarativeItem *root, QObject *parent = 0);
  ~WindowController();

public slots:
  void show();

  void setOrientation();

  void exposedContentRectChanged();

private:
  QWidget *m_view;
  Settings *m_settings;
  QDeclarativeItem *m_root;
};

#endif /* WINDOW_CONTROLLER_H */
