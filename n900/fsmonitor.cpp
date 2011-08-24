#include "fsmonitor.h"
#include <gio/gio.h>
#include <QDebug>

#define PATH "/home/user/MyDocs"

template<typename T> class GObjectContainer {
public:
  GObjectContainer(T *t) : m_t(t) {}
  ~GObjectContainer() { g_object_unref(m_t); }

  T *operator*() { return m_t; }

private:
  T *m_t;
};

void mount_added(GVolumeMonitor *volume_monitor, GMount *mount, gpointer user_data) {
  Q_UNUSED(volume_monitor);

  GObjectContainer<GFile> root(g_mount_get_root(mount));
  gchar *path = g_file_get_path(*root);

  if (QLatin1String(path) == QLatin1String(PATH)) {
    static_cast<FSMonitor *>(user_data)->setAvailable(true);
  }

  g_free(path);
}

void mount_removed(GVolumeMonitor *volume_monitor, GMount *mount, gpointer user_data) {
  Q_UNUSED(volume_monitor);

  GObjectContainer<GFile> root(g_mount_get_root(mount));
  gchar *path = g_file_get_path(*root);

  if (QLatin1String(path) == QLatin1String(PATH)) {
    static_cast<FSMonitor *>(user_data)->setAvailable(false);
  }

  g_free(path);
}


FSMonitor::FSMonitor(QObject *parent) : QObject(parent) {
  m_available = false;
  m_monitor = g_volume_monitor_get();

  GList *list = g_volume_monitor_get_mounts(m_monitor);

  for (unsigned x = 0; x < g_list_length(list); x++) {
    GMount *mount = (GMount *)g_list_nth_data(list, x);

    GObjectContainer<GFile> root(g_mount_get_root(mount));
    gchar *path = g_file_get_path(*root);

    if (QLatin1String(path) == QLatin1String(PATH)) {
      m_available = true;
    }

    if (m_available) {
      g_free(path);
      break;
    }
  }

  for (unsigned x = 0; x < g_list_length(list); x++) {
    g_object_unref(g_list_nth_data(list, x));
  }

  g_list_free(list);

  g_signal_connect(m_monitor, "mount-added", G_CALLBACK(mount_added), this);
  g_signal_connect(m_monitor, "mount-removed", G_CALLBACK(mount_removed), this);
}

FSMonitor::~FSMonitor() {
  g_object_unref(m_monitor);
}

bool FSMonitor::isAvailable() const {
  return m_available;
}

void FSMonitor::setAvailable(bool available) {
  if (m_available != available) {
    m_available = available;
    emit availabilityChanged();
  }
}
