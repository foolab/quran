#ifndef AUDIO_POLICY_H
#define AUDIO_POLICY_H

#include <QObject>

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
};

#endif /* AUDIO_POLICY_H */
