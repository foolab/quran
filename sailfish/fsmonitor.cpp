#include "fsmonitor.h"
#include <QDebug>

FSMonitor::FSMonitor(QObject *parent) : QObject(parent), m_available(true) {

}

FSMonitor::~FSMonitor() {

}

bool FSMonitor::isAvailable() const {
  return m_available;
}
