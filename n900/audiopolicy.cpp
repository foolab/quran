#include "audiopolicy.h"
#include <QDebug>

AudioPolicy::AudioPolicy(QObject *parent) : QObject(parent), m_acquired(false) {

  m_osso = osso_initialize("N900-Quran", VERSION, FALSE, NULL);
  m_playback = pb_playback_new_2((DBusConnection *)osso_get_dbus_connection(m_osso),
				 PB_CLASS_MEDIA,
				 PB_FLAG_AUDIO,
				 PB_STATE_STOP,
				 state_request_cb,
				 this);

  pb_playback_set_stream(m_playback, "Playback Stream");
}

AudioPolicy::~AudioPolicy() {
  if (m_playback) {
    pb_playback_destroy(m_playback);
  }

  if (m_osso) {
    osso_deinitialize(m_osso);
  }
}

void AudioPolicy::acquire() {
  if (m_acquired) {
    QMetaObject::invokeMethod(this, "acquired", Qt::QueuedConnection);
  }
  else {
    pb_playback_req_state(m_playback, PB_STATE_PLAY, state_reply_cb, this);
  }
}

void AudioPolicy::state_request_cb(pb_playback_t *pb, enum pb_state_e req_state,
				   pb_req_t* ext_req, void *data) {

  Q_UNUSED(pb);
  Q_UNUSED(ext_req);

  AudioPolicy *that = (AudioPolicy *)data;

  if (req_state == PB_STATE_STOP) {
    that->m_acquired = false;
    QMetaObject::invokeMethod(that, "lost", Qt::QueuedConnection);
  }
  else if (req_state == PB_STATE_PLAY) {
    that->m_acquired = true;
    QMetaObject::invokeMethod(that, "acquired", Qt::QueuedConnection);
  }
}

void AudioPolicy::state_reply_cb(pb_playback_t *pb, enum pb_state_e granted_state,
				 const char *reason, pb_req_t *req, void *data) {

  Q_UNUSED(reason);

  if (granted_state == PB_STATE_STOP) {
    QMetaObject::invokeMethod((AudioPolicy *)data, "denied", Qt::QueuedConnection);
  }
  else if (granted_state == PB_STATE_PLAY) {
    QMetaObject::invokeMethod((AudioPolicy *)data, "acquired", Qt::QueuedConnection);
  }

  pb_playback_req_completed(pb, req);
}
