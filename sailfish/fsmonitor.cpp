#include "fsmonitor.h"

FSMonitor::FSMonitor(QObject *parent) :
  QObject(parent) {

}

FSMonitor::~FSMonitor() {

}

bool FSMonitor::isAvailable() const {
  return true;
}
