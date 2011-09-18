#ifndef AUDIO_POLICY_H
#define AUDIO_POLICY_H

#include <QObject>
#include <libplayback/playback.h>
#include <dbus-1.0/dbus/dbus.h>
#include <libosso.h>

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
  static void state_request_cb(pb_playback_t *pb, enum pb_state_e req_state,
			       pb_req_t* ext_req, void *data);

  static void state_reply_cb(pb_playback_t *pb, enum pb_state_e granted_state,
			     const char *reason, pb_req_t *req, void *data);

  osso_context_t* m_osso;
  pb_playback_t *m_playback;

  bool m_acquired;
};

#endif /* AUDIO_POLICY_H */
