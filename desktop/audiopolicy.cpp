#include "audiopolicy.h"
#include <QDebug>

AudioPolicy::AudioPolicy(QObject *parent) :
  QObject(parent) {

}

AudioPolicy::~AudioPolicy() {

}

bool AudioPolicy::acquire() {
  QMetaObject::invokeMethod(this, "acquired", Qt::QueuedConnection);

  return true;
}

void AudioPolicy::release() {

}
