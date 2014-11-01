package org.foolab.quran;

import android.content.pm.ActivityInfo;
import android.util.Log;
import android.media.AudioManager;
import android.app.Activity;
import android.content.Context;

public class AndroidSupport implements AudioManager.OnAudioFocusChangeListener {
    private static String TAG = "AndroidSupport";

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

    public void acquireAudioFocus() {
	org.qtproject.qt5.android.QtNative.activity().runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    Activity activity = org.qtproject.qt5.android.QtNative.activity();
		    AudioManager am =
			(AudioManager)activity.getSystemService(Context.AUDIO_SERVICE);
		    int result = am.requestAudioFocus(AndroidSupport.this,
						      AudioManager.STREAM_MUSIC,
						      AudioManager.AUDIOFOCUS_GAIN);
		    if (result != AudioManager.AUDIOFOCUS_REQUEST_GRANTED) {
			audioFocusDenied();
		    } else {
			audioFocusAcquired();
		    }
		}
	    });
    }

    public void releaseAudioFocus() {
	org.qtproject.qt5.android.QtNative.activity().runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    Activity activity = org.qtproject.qt5.android.QtNative.activity();
		    AudioManager am =
			(AudioManager)activity.getSystemService(Context.AUDIO_SERVICE);
		    am.abandonAudioFocus(AndroidSupport.this);
		    audioFocusReleased();
		}
	    });
    }

    private void setOrientation(final int o) {
	org.qtproject.qt5.android.QtNative.activity().runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    org.qtproject.qt5.android.QtNative.activity().setRequestedOrientation(o);
		}
	    });
    }

    @Override
    public void onAudioFocusChange(int focusChange) {
	switch (focusChange) {
	case AudioManager.AUDIOFOCUS_GAIN:
	    audioFocusAcquired();
	    break;
	case AudioManager.AUDIOFOCUS_LOSS:
	case AudioManager.AUDIOFOCUS_LOSS_TRANSIENT:
	case AudioManager.AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK:
	    audioFocusLost();
	    break;
	}
    }

    native void audioFocusAcquired();
    native void audioFocusDenied();
    native void audioFocusLost();
    native void audioFocusReleased();
}
