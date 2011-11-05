#ifndef AUDIO_POLICY_H
#define AUDIO_POLICY_H

#include <QObject>

namespace ResourcePolicy {
  class ResourceSet;
  class AudioResource;
};

class AudioPolicy : public QObject {
  Q_OBJECT

public:
  AudioPolicy(QObject *parent = 0);
  ~AudioPolicy();

  void acquire();

signals:
  void acquired();
  void lost();
  void denied();

private:
  ResourcePolicy::ResourceSet *m_set;
  ResourcePolicy::AudioResource *m_audio;
};

#endif /* AUDIO_POLICY_H */
