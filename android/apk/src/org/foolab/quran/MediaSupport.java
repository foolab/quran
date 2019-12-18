/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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
import android.os.PowerManager.WakeLock;
import android.os.PowerManager;
import android.os.Handler;

public class MediaSupport implements AudioManager.OnAudioFocusChangeListener {
    private static String TAG = "QuranMediaSupport";

    AudioManager mAudioManager;
    PowerManager mPowerManager;
    WakeLock mLock;
    Handler mHandler;

    MediaSupport() {
	Context ctx = org.foolab.quran.MediaService.mCtx;
	mHandler = new Handler(ctx.getMainLooper());
	mAudioManager = (AudioManager)ctx.getSystemService(Context.AUDIO_SERVICE);
	mPowerManager = (PowerManager)ctx.getSystemService(Context.POWER_SERVICE);
	mLock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
    }

    public void acquireAudioFocus() {
	mHandler.post(new Runnable() {
		@Override
		public void run() {
		    int result = mAudioManager.requestAudioFocus(MediaSupport.this,
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
	mHandler.post(new Runnable() {
		@Override
		public void run() {
		    mAudioManager.abandonAudioFocus(MediaSupport.this);
		    audioFocusReleased();
		    mLock.release();
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
}
