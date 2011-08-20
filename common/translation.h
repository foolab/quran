#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QDeclarativeItem>
#include <QPointer>

class Translations;
class TranslationPrivate;

class Translation : public QDeclarativeItem {
  Q_OBJECT

  Q_ENUMS(Status);

  Q_PROPERTY(int tid READ tid WRITE setTid NOTIFY tidChanged);
  Q_PROPERTY(int downloadProgress READ downloadProgress NOTIFY downloadProgressChanged);
  Q_PROPERTY(QString error READ error() NOTIFY errorChanged);
  Q_PROPERTY(Status status READ status NOTIFY statusChanged);
  Q_PROPERTY(Translations * translations READ translations WRITE setTranslations);

  friend class TranslationPrivate;
  friend class Translations;

public:
  enum Status {
    None,
    Installed,
    Downloading,
    Error,
  };

  Translation(QDeclarativeItem *parent = 0);
  ~Translation();

  virtual void componentComplete();

  void setTid(int tid);
  int tid() const;

  void setTranslations(Translations *translations);
  Translations *translations() const;

  int downloadProgress() const;
  Status status() const;
  QString error() const;

signals:
  void tidChanged();
  void downloadProgressChanged();
  void statusChanged();
  void errorChanged();

private:
  int m_tid;

  TranslationPrivate *d_ptr;
  QPointer<Translations> m_translations;
};

#endif /* TRANSLATION_H */
