#include "mediaplaylist.h"
#include "recitation.h"
#include <QDebug>
#include "dataprovider.h"

MediaPlaylist::MediaPlaylist(Settings *settings, DataProvider *data, QObject *parent)
  : QMediaPlaylist(parent), m_settings(settings), m_data(data), m_recitation(0),
    m_chapter(0), m_page(0) {

}

MediaPlaylist::~MediaPlaylist() {

}

void MediaPlaylist::setRecitation(Recitation *recitation) {
  m_recitation = recitation;
}

MediaPlaylist::PlayMode MediaPlaylist::mode() {
  return m_mode;
}

int MediaPlaylist::page() {
  return m_page;
}

int MediaPlaylist::chapter() {
  return m_chapter;
}

void MediaPlaylist::playPage(int page) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayPage;
  m_page = page;

  Page p = m_data->pageFromIndex(page);
  QList<Fragment> frags = p.fragments();

  foreach (const Fragment& f, frags) {
    if (f.start() == 0) {
      Sura s = m_data->sura(f.sura());

      if (s.hasBasmala()) {
	addMedia(m_recitation->mediaUrl(1, 1));
      }
    }

    for (int x = f.start(); x < f.start() + f.size(); x++) {
      addMedia(m_recitation->mediaUrl(f.sura() + 1, x + 1));
    }
  }
}

void MediaPlaylist::playChapter(int chapter) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayChapter;
  m_chapter = chapter;

  Sura s = m_data->sura(chapter);

  if (s.hasBasmala()) {
    addMedia(m_recitation->mediaUrl(1, 1));
  }

  for (int x = 0; x < s.size(); x++) {
    addMedia(m_recitation->mediaUrl(chapter + 1, x + 1));
  }
}

void MediaPlaylist::playVerse(int chapter, int verse) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayVerse;

  addMedia(m_recitation->mediaUrl(chapter + 1, verse + 1));
}
