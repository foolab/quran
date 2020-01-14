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

package org.foolab.quran;

import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.content.Context;
import org.qtproject.qt5.android.QtNative;

public class NetworkManager {
    private static String TAG = "QuranNetworkManager";
    private WifiLock mLock;

    NetworkManager() {
	Context ctx = org.qtproject.qt5.android.QtNative.service();
	WifiManager manager = (WifiManager)ctx.getSystemService(Context.WIFI_SERVICE);
	mLock = manager.createWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, TAG);
	mLock.setReferenceCounted(true);
    }

    public void lock() {
	mLock.acquire();
    }

    public void unlock() {
	mLock.release();
    }
};
