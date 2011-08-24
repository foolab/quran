#ifndef FS_MONITOR_H
#define FS_MONITOR_H

#include <QObject>

typedef struct _GVolumeMonitor GVolumeMonitor;

class FSMonitor : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool available READ isAvailable NOTIFY availabilityChanged)

public:
  FSMonitor(QObject *parent = 0);
  ~FSMonitor();

  bool isAvailable() const;

  void setAvailable(bool available);

signals:
  void availabilityChanged();

private:
  bool m_available;
  GVolumeMonitor *m_monitor;
};

#endif /* FS_MONITOR_H */
