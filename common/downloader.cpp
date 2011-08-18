#include "downloader.h"
#include <QUrl>
#include <QNetworkRequest>

Downloader::Downloader(QObject *parent) : QNetworkAccessManager(parent) {

}

Downloader::~Downloader() {

}

QNetworkReply *Downloader::get(const QString& url) {
  return QNetworkAccessManager::get(QNetworkRequest(QUrl::fromEncoded(url.toUtf8())));
}
