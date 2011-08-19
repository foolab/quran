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

  emit installedChanged();
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
  // TODO:
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

void Translations::removeTranslation(int translation) {
  stopDownload(translation);

  // TODO:
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

#if 0
static QList<int> foo;
QList<int> Translations::installedTranslations() const {
  // TODO:
  if (foo.isEmpty()) {
    foo << 0 << 2 << 4;
  }

  return foo;
}

void Translations::removeTranslation(int translation) {
  // TODO:

  // TODO: stop download

  int idx = foo.indexOf(translation);
  if (idx != -1) {
    foo.takeAt(idx);
    emit installedTranslationsChanged();
  }
}


void Translations::startDownload(int tid) {
  TranslationInfo *inf = info(tid);

  if (!inf || inf->status == Translation::Downloading) {
    return;
  }

  // TODO:
  QNetworkReply *reply = m_downloader->get(QString("http://home.foolab.org/files/%1.txt").arg(translationId(tid)));

  reply->setProperty("tid", tid);

  QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
		   this, SLOT(replyDownloadProgress(qint64, qint64)));
  QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
		   this, SLOT(replyError()));
  QObject::connect(reply, SIGNAL(finished()),
		   this, SLOT(replyFinished()));

  if (!inf->startDownload(reply)) {
    // TODO: error;
    delete reply;

    inf->setError(tr("Download failed"));

    return;
  }

  inf->setStatus(Translation::Downloading);

  // TODO: add to installed.
}

void Translations::stopDownload(int tid) {
  // TODO:
  qCritical() << Q_FUNC_INFO << tid;
}

void Translations::registerTranslation(Translation *t) {
  TranslationInfo *inf = info(t->tid());

  if (!inf) {
    inf = new TranslationInfo(t->tid());
    if (installedTranslations().indexOf(t->tid()) != -1) {
      inf->status = Translation::Installed;
    }

    m_info << inf;
  }

  inf->add(t);

  if (inf->status != Translation::None) {
    t->statusChanged();
  }

  if (inf->downloadProgress != 0) {
    t->downloadProgressChanged();
  }
}

void Translations::unregisterTranslation(Translation *t) {
  TranslationInfo *inf = info(t->tid());

  if (!inf) {
    return;
  }

  inf->remove(t);

  if (inf->items.isEmpty() &&
      (inf->status == Translation::None || inf->status == Translation::Installed)) {

    m_info.takeAt(m_info.indexOf(inf));

    delete inf;
  }
}

int Translations::downloadProgress(int tid) {
  TranslationInfo *inf = info(tid);

  if (inf) {
    return inf->downloadProgress;
  }

  return 0;
}

Translation::Status Translations::status(int tid) {
  TranslationInfo *inf = info(tid);

  if (inf) {
    return inf->status;
  }

  return Translation::None;
}

QString Translations::error(int tid) {
  TranslationInfo *inf = info(tid);

  if (inf) {
    return inf->error;
  }

  return QString();
}

void Translations::replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
  TranslationInfo *inf = info(sender()->property("tid").toInt());
  if (!inf) {
    return;
  }

  int progress = ((bytesReceived * 100)/bytesTotal);

  inf->setDownloadProgress(progress);
}

void Translations::replyError() {
  TranslationInfo *inf = info(sender()->property("tid").toInt());
  if (!inf) {
    return;
  }

  qCritical() << Q_FUNC_INFO;
}

void Translations::replyFinished() {
  TranslationInfo *inf = info(sender()->property("tid").toInt());
  if (!inf) {
    return;
  }

  if (!inf->reply) {
    return;
  }

  if (inf->reply->error() != QNetworkReply::NoError) {
    // TODO: error

    return;
  }

  QList<QPair<quint64, quint64> > offsets;
  quint64 offset = 0;

  for (int x = 0; x < LINE_COUNT; x++) {
    if (!inf->reply->canReadLine()) {
      // TODO: error
    }

    QByteArray data = inf->reply->readLine();

    if (inf->file->write(data) != data.size()) {
      // TODO: error
    }

    offsets << qMakePair<quint64, quint64>(offset, data.size() - 1);
    offset += data.size();
  }

  if (inf->reply->readAll().size() != 0) {
    // TODO: error
  }

  qCritical() << inf->file->fileName() << offsets.size() << inf->reply->readAll().size();
  // TODO:
  //  inf->setStatus(Translation::Installed);
}

bool Translations::readData(TranslationInfo *info) {
  while (info->reply->canReadLine()) {
    QByteArray data = info->reply->readLine();

    if (!info->reply->error() != QNetworkReply::NoError) {
      return false;
    }

    if (info->file->write(data) != data.size()) {
      return false;
    }

    info->offsets << qMakePair<quint64, quint64>(info->offset, data.size() - 1);
    info->offset += data.size();
  }

  return true;
}

#if 0
QStringList Translations::installableTranslations() {
  QStringList res;

  for (int x = 0; x < TRANSLATIONS_LEN; x++) {
    res << QString::fromUtf8(Ts[x].id);
  }

  return res;
}
#endif

//void Translations::setEnabled(bool enabled) {

//}

//void Translations::refresh() {

//}

//QStringList Translations::installedTranslations() const {

//}

//QStringList languages();

//  Q_INVOKABLE QStringList installableTranslations(const QString& language) const;
#endif
