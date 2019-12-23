package org.foolab.quran;

import org.qtproject.qt5.android.bindings.QtService;
import org.foolab.quran.MediaSupport;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.IBinder;
import android.os.Messenger;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import android.os.RemoteException;

public class MediaService extends QtService {
    private static String TAG = "QuranMediaService";

    public static Context mCtx;

    /*
    public static final int ACTION_STATUS = 0;
    public static final int ACTION_STOP = 2;
    public static final int ACTION_PAUSE = 3;
    public static final int ACTION_RESUME = 4;

    public static final int MSG_STATUS = 0;
    public static final int MSG_STOP = 2;
    public static final int MSG_PAUSE = 3;
    public static final int MSG_RESUME = 4;

    public static final String CONFIG = "config";
    public static final String ACTION = "action";
    public static final String PLAYING = "playing";
    public static final String PAUSED = "paused";
    public static final String CHAPTER = "chapter";
    public static final String VERSE = "verse";
    public static final String STARTED = "started";
    public static final String ERROR = "error";

    private void reportStatus() {
	Log.e(TAG, "report status");
	Message reply = Message.obtain(null, MSG_STATUS, 0, 0);
	Bundle bundle = new Bundle(5);
	bundle.putBoolean(MediaService.PLAYING, isPlaying());
	bundle.putBoolean(MediaService.PAUSED, isPaused());
	bundle.putBoolean(MediaService.STARTED, m_id != -1);
	bundle.putBoolean(MediaService.ERROR, m_error);
	bundle.putInt(MediaService.CHAPTER, chapter());
	bundle.putInt(MediaService.VERSE, verse());
	reply.setData(bundle);

        try {
	    m_sender.send(reply);
        } catch (RemoteException e) {
	    Log.e(TAG, "failed to send message");
            e.printStackTrace();
        }

	Log.e(TAG, "reported status");
    }
    */
    @Override
    public void onCreate() {
	Log.e(TAG, "on create");
	super.onCreate();
	MediaService.mCtx = this;
    }
    /*
    @Override
    public void onDestroy() {
	Log.e(TAG, "on destroy");
	super.onDestroy();
    }

    public IBinder onBind(Intent intent) {
	Log.e(TAG, "on bind");
	return m_messenger.getBinder();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
	Log.e(TAG, "on start command");
	byte[] conf = intent.getByteArrayExtra(MediaService.CONFIG);
	play(conf);
	reportStatus();

	return START_NOT_STICKY;
    }

    private Messenger m_messenger = new Messenger(new Handler() {
	    @Override
	    public void handleMessage(Message msg) {
		Log.e(TAG, "handle message");
		switch (msg.what) {
		case MediaService.ACTION_STATUS:
		    m_sender = msg.replyTo;
		    reportStatus();
		    break;
		case MediaService.MSG_STOP:
		    stop();
		    reportStatus();
		    break;

		case MediaService.MSG_PAUSE:
		    pause();
		    reportStatus();
		    break;

		case MediaService.MSG_RESUME:
		    resume();
		    reportStatus();
		    break;

		default:
		    super.handleMessage(msg);
		}
	    }
	});

    //    private native void init();
    private native boolean play(byte[] conf);
    private native void stop();
    private native void pause();
    private native void resume();
    private native int chapter();
    private native int verse();

    private native boolean isPlaying();
    private native boolean isPaused();

    private Messenger m_sender;
    private boolean m_error = false;
    private int m_id = -1;

    */
}
