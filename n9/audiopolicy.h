#ifndef AUDIO_POLICY_H
#define AUDIO_POLICY_H

#include <QObject>
#include <policy/resource-set.h>

class AudioPolicy : public QObject {
  Q_OBJECT

public:
  AudioPolicy(QObject *parent = 0);
  ~AudioPolicy();

  void acquire();
  void release();

signals:
  void acquired();
  void lost();
  void denied();

private slots:
  void resourcesReleased();
  void lostResources();
  void resourcesGranted(const QList<ResourcePolicy::ResourceType>& optional);
  void resourcesDenied();

private:
  ResourcePolicy::ResourceSet *m_set;
};

#endif /* AUDIO_POLICY_H */
