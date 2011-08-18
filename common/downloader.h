#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QNetworkAccessManager>

class Downloader : public QNetworkAccessManager {
  Q_OBJECT

public:
  Downloader(QObject *parent = 0);
  ~Downloader();

  QNetworkReply *get(const QString& url);
};

#endif /* DOWNLOADER_H */
