#include "recitations.h"
#include "recitation.h"
#include <QDebug>
#include "settings.h"
#include <QMediaPlayer>

Recitations::Recitations(const QString& dir, Downloader *downloader, Settings *settings,
			 DataProvider *data, QObject *parent)
  : QObject(parent), m_downloader(downloader), m_dir(dir), m_settings(settings),
    m_data(data), m_player(0), m_recitation(0), m_current(0) {

}

Recitations::~Recitations() {
  qDeleteAll(m_installed.values());
  m_installed.clear();

  if (m_player) {
    m_player->stop();
  }

  delete m_player;
  m_player = 0;

  m_recitation = 0;
}

int Recitations::current() const {
  return m_current;
}

QList<int> Recitations::installed() const {
  return m_installed.keys();
}

void Recitations::refresh() {
  m_dir.mkpath(".");

  qDeleteAll(m_installed.values());
  m_installed.clear();

  QStringList entries(m_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

  int x = 0;

  foreach (const QString& entry, entries) {
    Recitation *r = Recitation::create(entry, m_dir.filePath(entry));
    if (r) {
      m_installed.insert(x++, r);
    }
  }

  emit installedChanged();
}

QString Recitations::recitationName(int id) {
  if (m_installed.contains(id)) {
    return m_installed[id]->name();
  }

  return QString();
}

bool Recitations::load(int id) {
  if (m_installed.isEmpty() || m_installed.keys().indexOf(id) == -1) {
    return false;
  }

  if (m_player) {
    delete m_player;
  }

  m_player = new QMediaPlayer(this);

  m_recitation = m_installed[id];

  m_current = id;

  m_settings->setDefaultRecitation(m_recitation->id());

  emit currentChanged();

  return true;
}

bool Recitations::loadDefault() {
  if (m_installed.isEmpty()) {
    return false;
  }

  QString id = m_settings->defaultRecitation();
  QList<int> keys = m_installed.keys();

  foreach (int val, keys) {
    if (m_installed[val]->id() == id) {
      return load(val);
    }
  }

  return load(keys[0]);
}

void Recitations::unload() {
  if (m_player) {
    m_player->stop();
  }

  delete m_player;
  m_player = 0;

  m_recitation = 0;
}

void Recitations::play(int chapter, int verse) {
  if (!m_player || !m_recitation) {
    return;
  }

  m_player->setMedia(m_recitation->mediaUrl(chapter + 1, verse + 1));
  m_player->play();
}
