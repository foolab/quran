#include "audiopolicy.h"
#include <QDebug>
#include <AudioResourceQt>

AudioPolicy::AudioPolicy(QObject *parent) :
  QObject(parent),
  m_resource(new AudioResourceQt::AudioResource(this, AudioResourceQt::AudioResource::MediaType)) {

  QObject::connect(m_resource, SIGNAL(acquiredChanged()), this, SLOT(acquiredChanged()));
}

AudioPolicy::~AudioPolicy() {

}

bool AudioPolicy::acquire() {
  return m_resource->acquire();
}

void AudioPolicy::release() {
  m_resource->release();
}

void AudioPolicy::acquiredChanged() {
  if (m_resource->isAcquired()) {
    emit acquired();
  }
  else {
    emit lost();
  }
}
