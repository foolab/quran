#include "translation.h"
#include "translations.h"
#include "translation_p.h"
#include <QDebug>

Translation::Translation(QObject *parent)
  : QObject(parent), m_tid(-1),
    d_ptr(0), m_translations(0) {

}

Translation::~Translation() {
  if (m_translations) {
    m_translations->unregisterTranslation(this);
  }

  d_ptr = 0;
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
  return d_ptr ? d_ptr->downloadProgress() : 0;
}

Translation::Status Translation::status() const {
  return d_ptr ? d_ptr->status() : Translation::None;
}

QString Translation::error() const {
  return d_ptr ? d_ptr->error() : QString();
}

void Translation::startDownload() {
  if (m_translations) {
    m_translations->startDownload(tid());
  }
}

void Translation::stopDownload() {
  if (m_translations) {
    m_translations->stopDownload(tid());
  }
}

void Translation::setTranslations(Translations *translations) {
  if (m_translations && d_ptr) {
    m_translations->unregisterTranslation(this);
  }

  m_translations = translations;

  if (tid() >= 0 && m_translations) {
    d_ptr = m_translations->registerTranslation(this);
  }
}

Translations *Translation::translations() const {
  return m_translations;
}
