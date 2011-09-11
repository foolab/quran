#ifndef RECITATIONS_H
#define RECITATIONS_H

#include <QObject>
#include <QDir>
#include <QMap>

class Downloader;
class Settings;
class DataProvider;
class Recitation;
class QMediaPlayer;

class Recitations : public QObject {
  Q_OBJECT

  Q_PROPERTY(QList<int> installed READ installed NOTIFY installedChanged);
  Q_PROPERTY(int current READ current NOTIFY currentChanged);

public:
  Recitations(const QString& dir, Downloader *downloader, Settings *settings,
	      DataProvider *data, QObject *parent = 0);

  ~Recitations();

  QList<int> installed() const;

  Q_INVOKABLE QString recitationName(int id);

  Q_INVOKABLE bool load(int id);
  Q_INVOKABLE bool loadDefault();

  int current() const;

public slots:
  void refresh();
  void play(int chapter, int verse);
  void unload();

signals:
  void installedChanged();
  void currentChanged();

private:
  Downloader *m_downloader;

  const QDir m_dir;

  Settings *m_settings;
  DataProvider *m_data;

  QMap<int, Recitation *> m_installed;

  QMediaPlayer *m_player;
  Recitation *m_recitation;
  int m_current;
};

#endif /* RECITATIONS_H */
