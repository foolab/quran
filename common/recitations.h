#ifndef RECITATIONS_H
#define RECITATIONS_H

#include <QObject>
#include <QDir>
#include <QMap>
#include <qmobilityglobal.h>

class Downloader;
class Settings;
class DataProvider;
class Recitation;
class QMediaPlayer;
class AudioPolicy;
class MediaPlaylist;

QTM_BEGIN_NAMESPACE
class QOrientationSensor;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE;

class Recitations : public QObject {
  Q_OBJECT

  Q_PROPERTY(QList<int> installed READ installed NOTIFY installedChanged);
  Q_PROPERTY(int current READ current NOTIFY currentChanged);
  Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playingChanged);
  Q_PROPERTY(int chapter READ chapter NOTIFY chapterChanged);
  Q_PROPERTY(int verse READ verse NOTIFY verseChanged);

public:
  Recitations(const QString& dir, Downloader *downloader, Settings *settings,
	      DataProvider *data, QObject *parent = 0);

  ~Recitations();

  QList<int> installed() const;

  Q_INVOKABLE QString recitationName(int id);

  Q_INVOKABLE bool load(int id);
  Q_INVOKABLE bool loadDefault();

  int current() const;

  bool isPlaying() const;

  int chapter() const;
  int verse() const;

public slots:
  void refresh();
  void play(int chapter, int verse);
  void unload();
  void playPage(int number);
  void playChapter(int chapter);
  void playPart(int part);
  void stop();

signals:
  void installedChanged();
  void currentChanged();
  void playingChanged();
  void error(const QString& msg);
  void positionChanged(int chapter, int verse);
  void verseChanged();
  void chapterChanged();

private slots:
  void playerStateChanged();
  void policyAcquired();
  void policyLost();
  void policyDenied();
  void playerError();
  void playerMediaChanged();
  void sensorReadingChanged();

private:
  void setChapter(int chapter);
  void setVerse(int verse);

  Downloader *m_downloader;

  const QDir m_dir;

  Settings *m_settings;
  DataProvider *m_data;

  QMap<int, Recitation *> m_installed;

  QMediaPlayer *m_player;
  MediaPlaylist *m_playlist;
  Recitation *m_recitation;
  int m_current;

  AudioPolicy *m_policy;
  bool m_play;

  int m_chapter;
  int m_verse;

  QOrientationSensor *m_sensor;
};

#endif /* RECITATIONS_H */
