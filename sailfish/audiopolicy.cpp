/*
 * Copyright (c) 2014 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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

bool AudioPolicy::acquire() {
  return m_set->acquire();
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
