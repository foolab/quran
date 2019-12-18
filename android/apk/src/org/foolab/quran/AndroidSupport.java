/*
 * Copyright (c) 2014-2019 Mohammed Sameer <msameer@foolab.org>.
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

import android.content.pm.ActivityInfo;
import android.util.Log;
import android.app.Activity;
import android.content.res.AssetManager;

public class AndroidSupport {
    private static String TAG = "QuranAndroidSupport";

    private Activity mActivity;

    AndroidSupport() {
	mActivity = org.qtproject.qt5.android.QtNative.activity();
	storeAssetManager(mActivity.getAssets());
    }

    public void unlockOrientation() {
	setOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
    }

    public void lockOrientationPortrait() {
	setOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

    public void lockOrientationLandscape() {
	setOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    }

    private void setOrientation(final int o) {
	mActivity.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    org.qtproject.qt5.android.QtNative.activity().setRequestedOrientation(o);
		}
	    });
    }

    native void storeAssetManager(AssetManager assetManager);
}
