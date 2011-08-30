#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include <QObject>
#include <QStringList>
#include "translation.h"
#include <QDir>

class Downloader;
class TranslationPrivate;
class Settings;
class DataProvider;

class Translations : public QObject {
  Q_OBJECT

  Q_PROPERTY(QList<int> installed READ installed NOTIFY installedChanged);
  Q_PROPERTY(QList<int> downloads READ downloads NOTIFY downloadsChanged);
  Q_PROPERTY(QList<int> categories READ categories NOTIFY categoriesChanged);
  Q_PROPERTY(QList<int> active READ active NOTIFY activeChanged);

  Q_PROPERTY(int current READ current NOTIFY currentChanged);

public:
  Translations(const QString& dir, Downloader *downloader, Settings *settings,
	       DataProvider *data, QObject *parent = 0);

  ~Translations();

  QList<int> installed() const;
  QList<int> downloads() const;
  QList<int> categories() const;
  QList<int> active() const;
  QList<int> error() const;

  int current() const;

  Q_INVOKABLE QString categoryName(int category);
  Q_INVOKABLE QList<int> translations(int category);
  Q_INVOKABLE QString translationName(int translation);

  void statusChanged(int tid, Translation::Status oldStatus, Translation::Status newStatus);

  Q_INVOKABLE bool load(int tid);
  Q_INVOKABLE bool loadDefault();

  TranslationPrivate *registerTranslation(Translation *t);
  void unregisterTranslation(Translation *t);

  QString index(int tid) const;
  QString data(int tid) const;

  Q_INVOKABLE QString id(int tid) const;

public slots:
  void refresh();
  void startDownload(int tid);
  void stopDownload(int tid);
  void removeTranslation(int tid);
  void unload();
  void stopDownloads();

signals:
  void downloadsChanged();
  void categoriesChanged();
  void activeChanged();
  void installedChanged();
  void currentChanged();

  void installed(int id);
  void failed(int id);
  void removed(int id);

private:
  TranslationPrivate *info(int tid);
  int tid(const QString& id);

  void setCurrent(int tid);

  Downloader *m_downloader;

  const QDir m_dir;

  Settings *m_settings;
  DataProvider *m_data;

  QList<TranslationPrivate *> m_info;

  QList<int> m_installed;

  int m_current;
};

#endif /* TRANSLATIONS_H */
