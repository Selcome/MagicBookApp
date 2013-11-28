package marshal.magicbookapp;

import static marshal.magicbookapp.Config.DELAY_START_DETECT_MOVE;
import static marshal.magicbookapp.Config.DETECT_MOVE_INTERVAL;
import static marshal.magicbookapp.Config.MAX_DETECT_NOT_MOVE_COUNT;
import static marshal.magicbookapp.Config.TAG;

import java.util.Iterator;
import java.util.Timer;
import java.util.TimerTask;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

public class Detector {

	private Timer timer;

	protected DetectData detectData;

	private LooperThread moveDetectLooper, matchDetectLooper;

	private long moveTimeStamp, matchTimeStamp;

	private int detectNotMoveCount;

	private DetectListener detectListener;

	static {
		System.loadLibrary("Detector");
	}

	public Detector(DetectData detectData, DetectListener detectListener) {
		this.detectData = detectData;
		this.detectListener = detectListener;
	}

	public void start() {
		Log.d(TAG, "move detector start..");
		moveDetectLooper = new LooperThread();
		moveDetectLooper.start();

		Log.d(TAG, "match detector start..");
		matchDetectLooper = new LooperThread();
		matchDetectLooper.start();

		timer = new Timer();
		timer.schedule(new DetectTimerTask(), DELAY_START_DETECT_MOVE,
				DETECT_MOVE_INTERVAL);

		init();
	}

	public void stop() {
		timer.cancel();

		Log.d(TAG, "move detector stop..");
		moveDetectLooper.mHandler.post(new Runnable() {
			@Override
			public void run() {
				Looper.myLooper().quit();
			}
		});

		Log.d(TAG, "match detector stop..");
		matchDetectLooper.mHandler.post(new Runnable() {
			@Override
			public void run() {
				Looper.myLooper().quit();
			}
		});

		clear();
	}

	private void detectMove() {
		Log.d(TAG, "detector move..");

		if (detectListener != null && detectData.getCurrentFrame() != null) {// 有可能检查移动的操作快于帧数据的设置
			detectListener.onMoveDetected();
			if (detectMove(detectData.getCurrentFrame())) {
				detectNotMoveCount = 0;
				moveTimeStamp = System.currentTimeMillis();
			} else {
				detectNotMoveCount++;
			}

			if (detectNotMoveCount == MAX_DETECT_NOT_MOVE_COUNT) {
				matchTimeStamp = moveTimeStamp;
				detectMatch();
			}
		}
	}

	private void detectMatch() {
		if (!matchDetectLooper.buzy && matchDetectLooper.running) {
			matchDetectLooper.mHandler.post(new Runnable() {
				@Override
				public void run() {
					matchDetectLooper.buzy = true;
					int matchIndex = detectMatch(detectData.getCurrentFrame());
					if (matchIndex >= 0 && matchTimeStamp == moveTimeStamp) {
						detectListener.onMatchDetected(matchIndex);
					}
					matchDetectLooper.buzy = false;
				}
			});
		}
	}

	public native void setSize(int width, int height);

	private native void init();

	private native void clear();

	private native boolean detectMove(byte[] currentFrame);

	private native int detectMatch(byte[] currentFrame);

	class LooperThread extends Thread {
		Handler mHandler;

		boolean running, buzy;

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
			if (!moveDetectLooper.buzy && moveDetectLooper.running) {
				moveDetectLooper.mHandler.post(new Runnable() {
					@Override
					public void run() {
						moveDetectLooper.buzy = true;
						detectMove();
						moveDetectLooper.buzy = false;
					}
				});
			}
		}
	}

}

interface DetectListener {
	void onMoveDetected();

	void onMatchDetected(int detectIndex);
}

interface DetectData {
	byte[] getCurrentFrame();

	Iterator getMatchImages();
}
