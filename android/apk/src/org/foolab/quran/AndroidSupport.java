package org.foolab.quran;

import android.content.pm.ActivityInfo;
import android.util.Log;
import android.media.AudioManager;
import android.app.Activity;
import android.content.Context;
import android.os.PowerManager.WakeLock;
import android.os.PowerManager;
import android.content.res.AssetManager;

public class AndroidSupport implements AudioManager.OnAudioFocusChangeListener {
    private static String TAG = "org.foolab.quran";

    AudioManager mAudioManager;
    PowerManager mPowerManager;
    Activity mActivity;
    WakeLock mLock;

    AndroidSupport() {
	mActivity = org.qtproject.qt5.android.QtNative.activity();
	mAudioManager = (AudioManager)mActivity.getSystemService(Context.AUDIO_SERVICE);
	mPowerManager = (PowerManager)mActivity.getSystemService(Context.POWER_SERVICE);
	mLock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
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

    public void acquireAudioFocus() {
	mActivity.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    int result = mAudioManager.requestAudioFocus(AndroidSupport.this,
						      AudioManager.STREAM_MUSIC,
						      AudioManager.AUDIOFOCUS_GAIN);
		    if (result != AudioManager.AUDIOFOCUS_REQUEST_GRANTED) {
			audioFocusDenied();
			mLock.release();
		    } else {
			audioFocusAcquired();
			mLock.acquire();
		    }
		}
	    });
    }

    public void releaseAudioFocus() {
	mActivity.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    mAudioManager.abandonAudioFocus(AndroidSupport.this);
		    audioFocusReleased();
		    mLock.release();
		}
	    });
    }

    private void setOrientation(final int o) {
	mActivity.runOnUiThread(new Runnable() {
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
	    mLock.acquire();
	    break;
	case AudioManager.AUDIOFOCUS_LOSS:
	case AudioManager.AUDIOFOCUS_LOSS_TRANSIENT:
	case AudioManager.AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK:
	    audioFocusLost();
	    mLock.release();
	    break;
	}
    }

    native void audioFocusAcquired();
    native void audioFocusDenied();
    native void audioFocusLost();
    native void audioFocusReleased();
    native void storeAssetManager(AssetManager assetManager);
}
