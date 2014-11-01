package org.foolab.quran;

import android.content.pm.ActivityInfo;

public class AndroidSupport {
    public static void unlockOrientation() {
	setOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
    }

    public static void lockOrientationPortrait() {
	setOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

    public static void lockOrientationLandscape() {
	setOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    }

    private static void setOrientation(final int o) {
	org.qtproject.qt5.android.QtNative.activity().runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    org.qtproject.qt5.android.QtNative.activity().setRequestedOrientation(o);
		}
	    });
    }
}
