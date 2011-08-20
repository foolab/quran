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
  QList<int> error() const;

  Q_INVOKABLE QString categoryName(int category);
  Q_INVOKABLE QList<int> translations(int category);
  Q_INVOKABLE QString translationName(int translation);

  void statusChanged(int tid, Translation::Status oldStatus, Translation::Status newStatus);
  //  void setEnabled(bool enabled);

  //  void refresh();

  //  Q_INVOKABLE QStringList installedTranslations() const;
  //  Q_INVOKABLE QStringList languages();

  //  Q_INVOKABLE QStringList installableTranslations(const QString& language) const;

  TranslationPrivate *registerTranslation(Translation *t);
  void unregisterTranslation(Translation *t);

  void refresh();

  QString index(int tid) const;
  QString data(int tid) const;

public slots:
  void startDownload(int tid);
  void stopDownload(int tid);
  void removeTranslation(int tid);

signals:
  void downloadsChanged();
  void categoriesChanged();
  void activeChanged();

private:
  TranslationPrivate *info(int tid);
  QString id(int tid) const;
  int tid(const QString& id);

  Downloader *m_downloader;

  const QDir m_dir;

  QList<TranslationPrivate *> m_info;

  QList<int> m_installed;
};

#endif /* TRANSLATIONS_H */
