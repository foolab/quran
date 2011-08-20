#include "translations.h"
#include "trans-meta.h"
#include <QLocale>
#include <QDebug>
#include "translation.h"
#include "downloader.h"
#include <QNetworkReply>
#include <QTemporaryFile>
#include "translation_p.h"
#include <QDebug>

#define INDEX_SUFFIX ".idx"
#define DATA_SUFFIX ".txt"
#define INDEX_FILTER "*" INDEX_SUFFIX

// TODO: limit the number of simultaneously downloaded translations.

Translations::Translations(const QString& dir, Downloader *downloader, QObject *parent)
  : QObject(parent), m_downloader(downloader), m_dir(dir) {

}

Translations::~Translations() {
  qDeleteAll(m_info);
}

TranslationPrivate *Translations::info(int tid) {
  foreach (TranslationPrivate *p, m_info) {
    if (p->tid() == tid) {
      return p;
    }
  }

  return 0;
}

void Translations::refresh() {
  m_dir.mkpath(".");

  QStringList list = m_dir.entryList(QStringList() << INDEX_FILTER,
				     QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

  m_installed.clear();

  foreach (const QString& file, list) {
    int tid = Translations::tid(QFileInfo(file).completeBaseName());
    if (m_installed.indexOf(tid) == -1) {
      m_installed << tid;
    }
  }

  emit activeChanged();
}

TranslationPrivate *Translations::registerTranslation(Translation *t) {
  TranslationPrivate *p = info(t->tid());

  if (!p) {
    p = new TranslationPrivate(m_installed.indexOf(t->tid()) == -1 ?
			       Translation::None : Translation::Installed, t->tid(), this);
    m_info << p;
  }

  p->add(t);

  return p;
}

void Translations::unregisterTranslation(Translation *t) {
  TranslationPrivate *p = t->d_ptr;

  if (!p) {
    return;
  }

  p->remove(t);

  if (p->canDestroy()) {
    m_info.takeAt(m_info.indexOf(p));
    delete p;
  }
}

QList<int> Translations::categories() const {
  QList<int> res;

  for (int x = 0; x < TRANSLATIONS_LEN; x++) {
    int language = Ts[x].language;
    if (res.indexOf(language) == -1) {
      res << language;
    }
  }

  qSort(res);

  return res;
}

QString Translations::categoryName(int category) {
  return QLocale::languageToString(static_cast<QLocale::Language>(category));
}

QList<int> Translations::translations(int category) {
  QList<int> res;

  for (int x = 0; x < TRANSLATIONS_LEN; x++) {
    if (Ts[x].language == category) {
      res << x;
    }
  }

  return res;
}

QString Translations::translationName(int translation) {
  return QString::fromUtf8(Ts[translation].name);
}

QList<int> Translations::installed() const {
  return m_installed;
}

QList<int> Translations::active() const {
  QList<int> res = m_installed;

  res += downloads();

  res += error();

  return res;
}

void Translations::startDownload(int tid) {
  TranslationPrivate *p = info(tid);
  if (!p || !p->canDownload()) {
    return;
  }

  // TODO:
  QNetworkReply *reply = m_downloader->get(QString("http://home.foolab.org/files/%1.txt").arg(id(tid)));

  if (!p->startDownload(reply)) {
    delete reply;
  }
}

void Translations::stopDownload(int tid) {
  TranslationPrivate *p = info(tid);
  if (!p) {
    return;
  }

  p->stopDownload();
}

QList<int> Translations::downloads() const {
  QList<int> res;

  foreach (const TranslationPrivate *p, m_info) {
    if (p->status() == Translation::Downloading) {
      res << p->tid();
    }
  }

  qSort(res);

  return res;
}

QList<int> Translations::error() const {
  QList<int> res;

  foreach (const TranslationPrivate *p, m_info) {
    if (p->status() == Translation::Error) {
      res << p->tid();
    }
  }

  qSort(res);

  return res;
}

void Translations::removeTranslation(int tid) {
  TranslationPrivate *p = info(tid);
  if (!p) {
    return;
  }

  p->stopDownload();

  p->remove();
}

QString Translations::id(int tid) const {
  return QString::fromUtf8(Ts[tid].id);
}

int Translations::tid(const QString& id) {
  for (int x = 0; x < TRANSLATIONS_LEN; x++) {
    if (QLatin1String(Ts[x].id) == id) {
      return x;
    }
  }

  return -1;
}

QString Translations::index(int tid) const {
  return QString("%1%2%3%4").arg(m_dir.absolutePath()).arg(QDir::separator())
    .arg(id(tid)).arg(INDEX_SUFFIX);
}

QString Translations::data(int tid) const {
  return QString("%1%2%3%4").arg(m_dir.absolutePath()).arg(QDir::separator())
    .arg(id(tid)).arg(DATA_SUFFIX);
}

void Translations::statusChanged(int tid, Translation::Status oldStatus,
				 Translation::Status newStatus) {

  TranslationPrivate *p = info(tid);
  if (!p) {
    return;
  }

  if (oldStatus == Translation::None && newStatus == Translation::Downloading) {
    // User initiated a download
    emit downloadsChanged();
    emit activeChanged();
  }
  else if (oldStatus == Translation::Installed && newStatus == Translation::None) {
    // User removed a translation
    m_installed.takeAt(m_installed.indexOf(p->tid()));
    emit activeChanged();
  }
  else if (oldStatus == Translation::Error && newStatus == Translation::Downloading) {
    // User restarted a failed download.
    emit activeChanged();
  }
  else if (oldStatus == Translation::Downloading && newStatus == Translation::None) {
    // User stopped a download
    emit downloadsChanged();
    emit activeChanged();
  }
  else if (oldStatus == Translation::Downloading && newStatus == Translation::Installed) {
    // Translation installed.
    m_installed.append(p->tid());
    emit activeChanged();
  }
}
