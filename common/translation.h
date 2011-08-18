#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>

class Translations;

class Translation : public QObject {
  Q_OBJECT

  Q_ENUMS(Status);

  Q_PROPERTY(int tid READ tid WRITE setTid NOTIFY tidChanged);
  Q_PROPERTY(int downloadProgress READ downloadProgress NOTIFY downloadProgressChanged);
  Q_PROPERTY(QString error READ error() NOTIFY errorChanged);
  Q_PROPERTY(Status status READ status NOTIFY statusChanged);
  Q_PROPERTY(Translations * translations READ translations WRITE setTranslations);

  friend class TranslationInfo;
  friend class Translations;

public:
  enum Status {
    None,
    Installed,
    Downloading,
    Error,
  };

  Translation(QObject *parent = 0);
  ~Translation();

  void setTid(int tid);
  int tid() const;

  void setTranslations(Translations *translations);
  Translations *translations() const;

  int downloadProgress() const;
  Status status() const;
  QString error() const;

public slots:
  void startDownload();
  void stopDownload();

signals:
  void tidChanged();
  void downloadProgressChanged();
  void statusChanged();
  void errorChanged();

private:
  int m_tid;

  Translations *m_translations;
};

#endif /* TRANSLATION_H */
