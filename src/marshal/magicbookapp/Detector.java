package marshal.magicbookapp;

import static marshal.magicbookapp.Config.*;

import java.util.Timer;
import java.util.TimerTask;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

public class Detector {

	private Timer timer;

	protected DetectData detectData;

	private byte[] previousFrame;

	private LooperThread detectLooper;

	private boolean buzy;

	private DetectListener detectListener;
	
	static{
		System.loadLibrary("Detector");
	}

	public Detector(DetectData detectData, DetectListener detectListener) {
		this.detectData = detectData;
		this.detectListener = detectListener;
	}

	public void start() {
		Log.d(TAG, "move detector start..");
		detectLooper = new LooperThread();
		detectLooper.start();

		timer = new Timer();
		timer.schedule(new DetectTimerTask(), DELAY_START_DETECT_MOVE,
				DETECT_MOVE_INTERVAL);
	}

	public void stop() {
		timer.cancel();
		Log.d(TAG, "move detector stop..");
		detectLooper.mHandler.post(new Runnable() {
			@Override
			public void run() {
				Looper.myLooper().quit();
			}
		});
	}

	private void detectMove() {
		Log.d(TAG, "detector move..");

		if (detectListener != null) {
			detectListener.onMoveDetected();
			detectMove(null);
		}
	}
	
	private native boolean detectMove(byte[] currentFrame);

	class LooperThread extends Thread {
		Handler mHandler;

		boolean running;

		@Override
		public void run() {
			Log.d(TAG, "looper thread run ..");
			running = true;
			Looper.prepare();
			mHandler = new Handler();
			Looper.loop();
			running = false;
		}
	}

	class DetectTimerTask extends TimerTask {
		@Override
		public void run() {
			if (!buzy) {
				detectLooper.mHandler.post(new Runnable() {
					@Override
					public void run() {
						detectMove();
					}
				});
			}
		}
	}

}

interface DetectListener {
	void onMoveDetected();
}
