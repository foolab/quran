package org.foolab.quran;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.util.Log;
import android.os.IBinder;
import android.os.Messenger;
import android.os.Handler;
import android.os.Message;
import android.os.RemoteException;
import android.os.Bundle;

public class MediaServiceControl {
    private static String TAG = "QuranMediaServiceControl";
    /*
    private Activity m_ctx;

    MediaServiceControl() {
	m_ctx = org.qtproject.qt5.android.QtNative.activity();
    }

    public void startService(Context ctx) {
	m_ctx.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    Log.e(TAG, "start service");
		    m_ctx.startService(new Intent(m_ctx, MediaService.class));
		}
	    });
    }

    public void stopService(Context ctx) {
	m_ctx.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    Log.e(TAG, "stop service");
		    m_ctx.startService(new Intent(m_ctx, MediaService.class));
		}
	    });
    }

    public void bindService() {
	m_ctx.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    Log.e(TAG, "bind service");
		    m_ctx.bindService(new Intent(m_ctx, MediaService.class),
				      m_conn, Context.BIND_AUTO_CREATE);
		}
	    });
    }

    public void unbindService(Context ctx) {
	m_ctx.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    Log.e(TAG, "unbind service");
		    m_ctx.unbindService(m_conn);
    		}
	    });
    }

    public void play(final byte[] conf) {
	m_ctx.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    Log.e(TAG, "play " + conf);
		    Intent i = new Intent(m_ctx, MediaService.class);
		    i.putExtra(MediaService.CONFIG, conf);
		    m_ctx.startService(i);
    		}
	    });
    }

    public void stop() {
	send(MediaService.ACTION_STOP);
    }

    public void pause() {
	send(MediaService.ACTION_PAUSE);
    }

    public void resume() {
	send(MediaService.ACTION_RESUME);
    }

    private void setPlaying(boolean playing) {
	if (m_playing != playing) {
	    m_playing = playing;
	    handlePlayingChanged();
	}
    }

    public boolean isPlaying() {
	return m_playing;
    }

    private void setPaused(boolean paused) {
	if (m_paused != paused) {
	    m_paused = paused;
	    handlePausedChanged();
	}
    }

    public boolean isPaused() {
	return false;
    }

    public boolean isConnected() {
	return m_sender != null;
    }

    private native void handlePlayingChanged();
    private native void handlePausedChanged();
    private native void handleError();
    private native void handlePositionChanged(int chapter, int verse);

    private ServiceConnection m_conn = new ServiceConnection() {
	    @Override
	    public void onServiceConnected(ComponentName name, IBinder service) {
		Log.e(TAG, "service connected");
		createMessenger();
		m_sender = new Messenger(service);
		send(MediaService.ACTION_STATUS);
	    }

	    @Override
	    public void onServiceDisconnected(ComponentName name) {
		Log.e(TAG, "service disconnected");
	    }
	};

    private Messenger m_messenger;

    // We need to create our Messenger object in the Android main thread
    // If we create it in the constructor or we create it when we declare the variable
    // then it will be in the Qt main thread which will
    // not work (We do not get messages from the service).
    private void createMessenger() {
	m_messenger = new Messenger(new Handler() {
		@Override
		public void handleMessage(Message msg) {
		    Log.e(TAG, "handle message " + Thread.currentThread().getName());
		    switch (msg.what) {
		    case MediaService.MSG_STATUS:
			Bundle b = msg.getData();
			setPlaying(b.getBoolean(MediaService.PLAYING));
			setPaused(b.getBoolean(MediaService.PAUSED));
			handlePositionChanged(b.getInt(MediaService.CHAPTER),
					       b.getInt(MediaService.VERSE));
			if (b.getBoolean(MediaService.ERROR)) {
			    handleError();
			}
		    default:
			super.handleMessage(msg);
		    }
		}
	    });

    }

    private void send(final int code) {
	m_ctx.runOnUiThread(new Runnable() {
		@Override
		public void run() {
		    Log.e(TAG, "send");
		    Message msg = Message.obtain(null, code, 0, 0);
		    msg.replyTo = m_messenger;
		    try {
			m_sender.send(msg);
		    } catch (RemoteException e) {
			Log.e(TAG, "failed to send message");
			e.printStackTrace();
		    }
		}
	    });
    }

    private boolean m_playing = false;
    private boolean m_paused = false;
    private Messenger m_sender;
    */
}
