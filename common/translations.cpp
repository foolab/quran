#include "translations.h"
#include "trans-meta.h"
#include <QLocale>
#include <QDebug>
#include "translation.h"
#include "downloader.h"
#include <QNetworkReply>
#include <QTemporaryFile>

#define LINE_COUNT 6236

class TranslationInfo {
public:
  TranslationInfo(int t) : tid(t), status(Translation::None),
			   downloadProgress(0), reply(0), file(0), offset(0) {}

  ~TranslationInfo() {
    if (reply) {
      reply->abort();
      delete reply;
      reply = 0;
    }

    if (file) {
      file->remove();
      delete file;
      file = 0;
    }
  }

  void setDownloadProgress(int dp) {
    if (downloadProgress != dp) {
      downloadProgress = dp;
      foreach(Translation *t, items) {
	t->downloadProgressChanged();
      }
    }
  }

  void setStatus(Translation::Status st) {
    if (status != st) {
      status = st;
      foreach(Translation *t, items) {
	t->statusChanged();
      }
    }
  }

  void setError(const QString& err) {
    error = err;

    setStatus(Translation::Error);

    foreach(Translation *t, items) {
      t->errorChanged();
    }
  }

  void add(Translation *t) {
    if (items.indexOf(t) == -1) {
      items << t;
    }
  }

  void remove(Translation *t) {
    int index = items.indexOf(t);
    if (index != -1) {
      items.takeAt(index);
    }
  }

  bool startDownload(QNetworkReply *r) {
    QTemporaryFile *f = new QTemporaryFile;

    if (!f->open()) {
      delete f;
      return false;
    }

    reply = r;
    file = f;

    offset = 0;
    offsets.clear();

    return true;
  }

  const int tid;
  QList<Translation *> items;
  Translation::Status status;
  int downloadProgress;
  QNetworkReply *reply;
  QTemporaryFile *file;
  QString error;
  quint64 offset;
  QList<QPair<quint64, quint64> > offsets;
};

Translations::Translations(const QString& dir, Downloader *downloader, QObject *parent)
  : QObject(parent), m_downloader(downloader), m_dir(dir) {
  // TODO:
}

Translations::~Translations() {
  // TODO: Abort downloads
  qDeleteAll(m_info);
}

TranslationInfo *Translations::info(int tid) {
  foreach (TranslationInfo *info, m_info) {
    if (info->tid == tid) {
      return info;
    }
  }

  return 0;
}

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

QList<int> Translations::categories() const {
  // TODO:

  QList<int> res;

  //  res << -1;

  for (int x = 0; x < TRANSLATIONS_LEN; x++) {
    int language = Ts[x].language;
    if (res.indexOf(language) == -1) {
      res << language;
    }
  }

  qSort(res);

  return res;
}


QString Translations::translationId(int tid) {
  return QString::fromUtf8(Ts[tid].id);
}

QString Translations::categoryName(int category) {
  //  if (category == -1) {
  //    return tr("Installed");
  //  }

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
