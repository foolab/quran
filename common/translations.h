#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include <QObject>
#include <QStringList>
#include "translation.h"
#include <QDir>

class Downloader;
class TranslationInfo;
class TranslationInfo;

class Translations : public QObject {
  Q_OBJECT

  Q_PROPERTY(QList<int> installedTranslations READ installedTranslations NOTIFY installedTranslationsChanged);
  Q_PROPERTY(QList<int> categories READ categories NOTIFY categoriesChanged);

public:
  Translations(const QString& dir, Downloader *downloader, QObject *parent = 0);
  ~Translations();

  QList<int> installedTranslations() const;

  QList<int> categories() const;

  Q_INVOKABLE QString categoryName(int category);
  Q_INVOKABLE QList<int> translations(int category);
  Q_INVOKABLE QString translationName(int translation);

  //  Q_INVOKABLE QStringList installableTranslations();

  //  void setEnabled(bool enabled);

  //  void refresh();

  //  Q_INVOKABLE QStringList installedTranslations() const;
  //  Q_INVOKABLE QStringList languages();

  //  Q_INVOKABLE QStringList installableTranslations(const QString& language) const;

  void registerTranslation(Translation *t);
  void unregisterTranslation(Translation *t);

  int downloadProgress(int tid);
  Translation::Status status(int tid);
  QString error(int tid);

public slots:
  void removeTranslation(int translation);
  void startDownload(int tid);
  void stopDownload(int tid);

signals:
  void installedTranslationsChanged();
  void categoriesChanged();

private slots:
  void replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void replyError();
  void replyFinished();

private:
  TranslationInfo *info(int tid);
  QString translationId(int tid);

  bool readData(TranslationInfo *info);

  Downloader *m_downloader;

  const QDir m_dir;

  QList<TranslationInfo *> m_info;
};

#endif /* TRANSLATIONS_H */
