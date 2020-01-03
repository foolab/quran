/*
 * Copyright (c) 2019-2020 Mohammed Sameer <msameer@foolab.org>.
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
import android.media.AudioManager;
import android.content.Context;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.PowerManager.WakeLock;
import android.os.PowerManager;
import android.os.Handler;
import org.qtproject.qt5.android.QtNative;

public class MediaSupport implements AudioManager.OnAudioFocusChangeListener {
    private static String TAG = "QuranMediaSupport";

    AudioManager mAudioManager;
    PowerManager mPowerManager;
    WakeLock mLock;
    Handler mHandler;
    NoisyReceiver mReceiver;

    private class NoisyReceiver extends BroadcastReceiver {
	private boolean mRegistered = false;

	@Override
	public void onReceive(Context context, Intent intent) {
	    if (AudioManager.ACTION_AUDIO_BECOMING_NOISY.equals(intent.getAction())) {
		handleFocusLost();
	    }
	}

	void register() {
	    if (!mRegistered) {
		IntentFilter filter = new IntentFilter(AudioManager.ACTION_AUDIO_BECOMING_NOISY);
		org.qtproject.qt5.android.QtNative.service().registerReceiver(this,
									      filter);
		mRegistered = true;
	    }
	}

	void unregister() {
	    if (mRegistered) {
		org.qtproject.qt5.android.QtNative.service().unregisterReceiver(this);
		mRegistered = false;
	    }
	}
    }

    MediaSupport() {
	Context ctx = org.qtproject.qt5.android.QtNative.service();
	mHandler = new Handler(ctx.getMainLooper());
	mAudioManager = (AudioManager)ctx.getSystemService(Context.AUDIO_SERVICE);
	mPowerManager = (PowerManager)ctx.getSystemService(Context.POWER_SERVICE);
	mLock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
	mReceiver = new NoisyReceiver();
    }

    public void acquireAudioFocus() {
	mHandler.post(new Runnable() {
		@Override
		public void run() {
		    int result = mAudioManager.requestAudioFocus(MediaSupport.this,
								 AudioManager.STREAM_MUSIC,
								 AudioManager.AUDIOFOCUS_GAIN);
		    if (result != AudioManager.AUDIOFOCUS_REQUEST_GRANTED) {
			handleFocusDenied();
		    } else {
			handleFocusAcquired();
		    }
		}
	    });
    }

    public void releaseAudioFocus() {
	mHandler.post(new Runnable() {
		@Override
		public void run() {
		    mAudioManager.abandonAudioFocus(MediaSupport.this);
		    mReceiver.unregister();
		    audioFocusReleased();
		    if (mLock.isHeld()) {
			mLock.release();
		    }
		}
	    });
    }

    @Override
    public void onAudioFocusChange(int focusChange) {
	switch (focusChange) {
	case AudioManager.AUDIOFOCUS_GAIN:
	    handleFocusAcquired();
	    break;
	case AudioManager.AUDIOFOCUS_LOSS:
	case AudioManager.AUDIOFOCUS_LOSS_TRANSIENT:
	case AudioManager.AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK:
	    handleFocusLost();
	    break;
	}
    }

    private void handleFocusAcquired() {
	if (!mLock.isHeld()) {
	    mLock.acquire();
	}

	audioFocusAcquired();
	mReceiver.register();
    }

    private void handleFocusLost() {
	audioFocusLost();
	mReceiver.unregister();
	if (mLock.isHeld()) {
	    mLock.release();
	}
    }

    private void handleFocusDenied() {
	audioFocusDenied();
	mReceiver.unregister();
	if (mLock.isHeld()) {
	    mLock.release();
	}
    }

    native void audioFocusAcquired();
    native void audioFocusDenied();
    native void audioFocusLost();
    native void audioFocusReleased();
}
