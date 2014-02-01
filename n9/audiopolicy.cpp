#include "audiopolicy.h"
#include <QDebug>

AudioPolicy::AudioPolicy(QObject *parent) :
  QObject(parent),
  m_set(new ResourcePolicy::ResourceSet("player", this)) {

  m_set->setAlwaysReply();

  ResourcePolicy::AudioResource *audio = new ResourcePolicy::AudioResource("player");
  audio->setProcessID(QCoreApplication::applicationPid());
  audio->setStreamTag("media.name", "*");
  audio->setOptional(false);
  m_set->addResourceObject(audio);

  QObject::connect(m_set, SIGNAL(resourcesReleased()), this, SLOT(resourcesReleased()));
  QObject::connect(m_set, SIGNAL(lostResources()), this, SLOT(lostResources()));
  QObject::connect(m_set, SIGNAL(resourcesGranted(const QList<ResourcePolicy::ResourceType>&)),
                   this, SLOT(resourcesGranted(const QList<ResourcePolicy::ResourceType>&)));
  QObject::connect(m_set, SIGNAL(resourcesDenied()), this, SLOT(resourcesDenied()));
}

AudioPolicy::~AudioPolicy() {

}

void AudioPolicy::acquire() {
  m_set->acquire();
}

void AudioPolicy::release() {
  m_set->release();
}

void AudioPolicy::resourcesReleased() {

}

void AudioPolicy::lostResources() {
  emit lost();
}

void AudioPolicy::resourcesGranted(const QList<ResourcePolicy::ResourceType>& optional) {
  Q_UNUSED(optional);

  emit acquired();
}

void AudioPolicy::resourcesDenied() {
  emit denied();
}
