#include "audiopolicy.h"
#include <QDebug>
#include <policy/resource-set.h>
#include <policy/audio-resource.h>

AudioPolicy::AudioPolicy(QObject *parent) : QObject(parent) {
  m_set = new ResourcePolicy::ResourceSet("player", this, true, true);
  m_audio = new ResourcePolicy::AudioResource("player");
  m_set->addResourceObject(m_audio);

  QObject::connect(m_set, SIGNAL(resourcesGranted(const QList<ResourcePolicy::ResourceType> &)),
		   this, SIGNAL(acquired()), Qt::QueuedConnection);
  QObject::connect(m_set, SIGNAL(resourcesDenied()),
		   this, SIGNAL(denied()), Qt::QueuedConnection);
  QObject::connect(m_set, SIGNAL(lostResources()),
		   this, SIGNAL(lost()), Qt::QueuedConnection);
}

AudioPolicy::~AudioPolicy() {
  m_set->release();
}

void AudioPolicy::acquire() {
  if (m_audio->isGranted()) {
    QMetaObject::invokeMethod(this, "acquired", Qt::QueuedConnection);
  }
  else {
    m_set->acquire();
  }
}
#if 0
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
#endif
