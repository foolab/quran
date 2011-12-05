#include "audiopolicy.h"
#include <QDebug>

AudioPolicy::AudioPolicy(QObject *parent) : QObject(parent) {
}

AudioPolicy::~AudioPolicy() {

}

void AudioPolicy::acquire() {
  QMetaObject::invokeMethod(this, "acquired", Qt::QueuedConnection);
}
