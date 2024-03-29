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

import org.qtproject.qt5.android.bindings.QtService;
import org.qtproject.qt5.android.QtNative;
import android.content.Intent;
import android.util.Log;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.Notification.Action;
import android.app.Notification.Action.Builder;

public class MediaService extends QtService {
    private static String TAG = "QuranMediaService";
    private int mStartId;
    private String mReciter;

    private static final int NOTIFICATION_ID = 13;
    private static final String ACTION_PLAY = "play";
    private static final String ACTION_STOP = "stop";
    private static final String ACTION_PAUSE = "pause";
    private static final String ACTION_RESUME = "resume";
    private static final String RECITER = "reciter";

    public void _stopService() {
	Log.d(TAG, "stop service");
	stopForeground(true);
	stopSelf(mStartId);
    }

    @Override
    public void onCreate() {
	Log.d(TAG, "on create");
	super.onCreate();
    }

    @Override
    public void onDestroy() {
	Log.d(TAG, "on destroy");
	super.onDestroy();
    }

    private Notification.Action makeNotificationAction(String action) {
	Intent intent = new Intent(this, MediaService.class);
	intent.setAction(action);

	PendingIntent actionIntent =
	    PendingIntent.getService(this, 0, intent,
				     PendingIntent.FLAG_UPDATE_CURRENT |
				     PendingIntent.FLAG_ONE_SHOT);

	return new Notification.Action.Builder(0, action, actionIntent).build();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
	Log.d(TAG, "on start command");

	mStartId = startId;

	if (intent == null) {
	    // TODO: handle
	}

	if (!onStartCommand(intent)) {
	    _stopService();
	    return START_STICKY;
	}

	String action = intent.getAction();
	if (action.equals(MediaService.ACTION_PLAY)) {
	    mReciter = intent.getStringExtra(MediaService.RECITER);
	}

	android.app.Notification n = updateNotification(action);

	startForeground(MediaService.NOTIFICATION_ID, n);

	return START_STICKY;
    }

    private Notification updateNotification(String action) {
	Intent launchIntent =
	    new Intent(this, org.qtproject.qt5.android.bindings.QtActivity.class);
	PendingIntent contentIntent =
	    PendingIntent.getActivity(this, 0, launchIntent, PendingIntent.FLAG_UPDATE_CURRENT);

	android.app.Notification.Builder builder =
	    new android.app.Notification.Builder(org.qtproject.qt5.android.QtNative.service());
	builder.setContentTitle(getString(R.string.app_name))
	    .setContentText(mReciter)
	    .setSmallIcon(R.drawable.icon)
	    .setShowWhen(false)
	    .setContentIntent(contentIntent)
	    .addAction(makeNotificationAction(MediaService.ACTION_STOP));

	// stop action has been handled by our native layer.
	if (action.equals(MediaService.ACTION_PLAY)) {
	    builder.addAction(makeNotificationAction(MediaService.ACTION_PAUSE));
	} else if (action.equals(MediaService.ACTION_PAUSE)) {
	    builder.addAction(makeNotificationAction(MediaService.ACTION_RESUME));
	} else if (action.equals(MediaService.ACTION_RESUME)) {
	    builder.addAction(makeNotificationAction(MediaService.ACTION_PAUSE));
	}

	return builder.build();
    }

    private native boolean onStartCommand(Intent intent);
}
