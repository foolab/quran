#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include <QObject>
#include <QStringList>
#include "translation.h"
#include <QDir>

#define LINE_COUNT 6236

class Downloader;
class TranslationPrivate;

class Translations : public QObject {
  Q_OBJECT

  Q_PROPERTY(QList<int> installed READ installed NOTIFY installedChanged);
  Q_PROPERTY(QList<int> downloads READ downloads NOTIFY downloadsChanged);
  Q_PROPERTY(QList<int> categories READ categories NOTIFY categoriesChanged);
  Q_PROPERTY(QList<int> active READ active NOTIFY activeChanged);

public:
  Translations(const QString& dir, Downloader *downloader, QObject *parent = 0);
  ~Translations();

  QList<int> installed() const;
  QList<int> downloads() const;
  QList<int> categories() const;
  QList<int> active() const;

  Q_INVOKABLE QString categoryName(int category);
  Q_INVOKABLE QList<int> translations(int category);
  Q_INVOKABLE QString translationName(int translation);

  //  void setEnabled(bool enabled);

  //  void refresh();

  //  Q_INVOKABLE QStringList installedTranslations() const;
  //  Q_INVOKABLE QStringList languages();

  //  Q_INVOKABLE QStringList installableTranslations(const QString& language) const;

  TranslationPrivate *registerTranslation(Translation *t);
  void unregisterTranslation(Translation *t);

  int downloadProgress(int tid);
  Translation::Status status(int tid);
  QString error(int tid);

  void startDownload(int tid);
  void stopDownload(int tid);

  void refresh();

  QString index(int tid) const;
  QString data(int tid) const;

public slots:
  void removeTranslation(int translation);

signals:
  void installedChanged();
  void downloadsChanged();
  void categoriesChanged();

#if 0
private slots:
  void replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void replyError();
  void replyFinished();

#endif

private:
  TranslationPrivate *info(int tid);
  QString id(int tid) const;
  int tid(const QString& id);

  //  bool readData(TranslationInfo *info);

  Downloader *m_downloader;

  const QDir m_dir;

  QList<TranslationPrivate *> m_info;

  QList<int> m_installed;
};

#endif /* TRANSLATIONS_H */
