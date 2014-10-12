#ifndef FS_MONITOR_H
#define FS_MONITOR_H

#include <QObject>

class FSMonitor : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool available READ isAvailable NOTIFY availabilityChanged)

public:
  FSMonitor(QObject *parent = 0);
  ~FSMonitor();

  bool isAvailable() const;

signals:
  void availabilityChanged();
};

#endif /* FS_MONITOR_H */
