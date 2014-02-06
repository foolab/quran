#ifndef AUDIO_POLICY_H
#define AUDIO_POLICY_H

#include <QObject>

namespace AudioResourceQt {
  class AudioResource;
};

class AudioPolicy : public QObject {
  Q_OBJECT

public:
  AudioPolicy(QObject *parent = 0);
  ~AudioPolicy();

  bool acquire();
  void release();

signals:
  void acquired();
  void lost();
  void denied();

private slots:
  void acquiredChanged();

private:
  AudioResourceQt::AudioResource *m_resource;
};

#endif /* AUDIO_POLICY_H */
