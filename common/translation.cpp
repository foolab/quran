#include "translation.h"
#include "translations.h"
#include <QDebug>

Translation::Translation(QObject *parent)
  : QObject(parent), m_tid(-1),
    m_translations(0) {

}

Translation::~Translation() {
  if (m_translations) {
    m_translations->unregisterTranslation(this);
  }
}

void Translation::setTid(int tid) {
  if (m_tid != tid) {
    m_tid = tid;

    emit tidChanged();

    setTranslations(m_translations);
  }
}

int Translation::tid() const {
  return m_tid;
}

int Translation::downloadProgress() const {
  if (m_translations) {
    return m_translations->downloadProgress(tid());
  }

  return 0;
}

Translation::Status Translation::status() const {
  if (m_translations) {
    return m_translations->status(tid());
  }

  return Translation::None;
}

QString Translation::error() const {
  if (m_translations) {
    return m_translations->error(tid());
  }

  return QString();
}

void Translation::startDownload() {
  if (m_translations && tid() >= 0) {
    m_translations->startDownload(tid());
  }
}

void Translation::stopDownload() {
  if (m_translations && tid() >= 0) {
    m_translations->stopDownload(tid());
  }
}

void Translation::setTranslations(Translations *translations) {
  if (m_translations) {
    m_translations->unregisterTranslation(this);
  }

  m_translations = translations;

  if (tid() >= 0 && m_translations) {
    m_translations->registerTranslation(this);
  }
}

Translations *Translation::translations() const {
  return m_translations;
}
