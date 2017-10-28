#include "audiopolicy.h"
#include "androidsupport.h"
#include <QDebug>

AudioPolicy::AudioPolicy(QObject *parent) :
  QObject(parent) {

}

AudioPolicy::~AudioPolicy() {

}

bool AudioPolicy::acquire() {
  AndroidSupport::acquireAudioFocus(this);
  return true;
}

void AudioPolicy::release() {
  AndroidSupport::releaseAudioFocus();
}
