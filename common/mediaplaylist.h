#ifndef MEDIA_PLAYLIST_H
#define MEDIA_PLAYLIST_H

#include <QMediaPlaylist>

class Settings;
class DataProvider;
class Recitation;

class MediaPlaylist : public QMediaPlaylist {
  Q_OBJECT

public:
  enum PlayMode {
    PlayPage,
    PlayChapter,
    PlayVerse,
    PlayPart,
  };

  MediaPlaylist(Settings *settings, DataProvider *data, QObject *parent = 0);
  ~MediaPlaylist();

  void setRecitation(Recitation *recitation);

  void playPage(int page);
  void playChapter(int chapter);
  void playVerse(int chapter, int verse);
  void playPart(int part);

  PlayMode mode();

  int page();
  int chapter();
  int part();

private:
  Settings *m_settings;
  DataProvider *m_data;
  Recitation *m_recitation;

  PlayMode m_mode;

  int m_chapter;
  int m_page;
  int m_part;
};

#endif /* MEDIA_PLAYLIST_H */
