#include "translation.h"
#include "translations.h"
#include "translation_p.h"
#include <QDebug>

Translation::Translation(QDeclarativeItem *parent)
  : QDeclarativeItem(parent), m_tid(-1),
    d_ptr(0), m_translations(0) {
}

Translation::~Translation() {
  if (m_translations) {
    m_translations->unregisterTranslation(this);
  }

  d_ptr = 0;
}

void Translation::componentComplete() {
  QDeclarativeItem::componentComplete();

  d_ptr = m_translations->registerTranslation(this);
}

void Translation::setTid(int tid) {
  if (m_tid != tid) {
    m_tid = tid;

    emit tidChanged();
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

void Translation::setTranslations(Translations *translations) {
  m_translations = translations;
}

Translations *Translation::translations() const {
  return m_translations;
}
