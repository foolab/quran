package org.foolab.quran;

import android.content.pm.ActivityInfo;
import android.util.Log;

public class AndroidSupport {
    AndroidSupport() {

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
	org.qtproject.qt5.android.QtNative.activity().runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    org.qtproject.qt5.android.QtNative.activity().setRequestedOrientation(o);
		}
	    });
    }
}
