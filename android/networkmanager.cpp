/*
 * Copyright (c) 2020 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "networkmanager.h"
#include <QAndroidJniObject>
#include <QAndroidJniExceptionCleaner>

class NetworkManagerPrivate {
public:
  QAndroidJniObject m_obj;
};

NetworkManager::NetworkManager() :
  d_ptr(new NetworkManagerPrivate) {

  d_ptr->m_obj = QAndroidJniObject("org/foolab/quran/NetworkManager", "()V");
  QAndroidJniExceptionCleaner cleaner;
}

NetworkManager::~NetworkManager() {
  delete d_ptr;
  d_ptr = 0;
}

void NetworkManager::ref() {
  QAndroidJniExceptionCleaner cleaner;

  d_ptr->m_obj.callMethod<void>("lock");
}

void NetworkManager::unref() {
  QAndroidJniExceptionCleaner cleaner;

  d_ptr->m_obj.callMethod<void>("unlock");
}
