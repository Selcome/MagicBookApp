package marshal.magicbookapp;

import static marshal.magicbookapp.Config.*;

import java.io.IOException;
import java.util.Iterator;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MagicView extends SurfaceView implements SurfaceHolder.Callback,
		PreviewCallback, DetectData, DetectListener {

	private Camera camera;

	private Detector detector;

	private Timer timer;

	private byte[] currentFrame;

	public MagicView(Context context) {
		super(context);
		getHolder().addCallback(this);
	}

	public void resume() {
		Log.d(TAG, "magic view resume.");
	}

	public void pause() {
		Log.d(TAG, "magic view pause.");
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {

		Camera.Parameters params = camera.getParameters();

		// 暂不支持自动适配，需要设置长宽的常量
		params.setPreviewSize(CAMERA_PREVIEW_WIDTH, CAMERA_PREVIEW_HEIGHT);

		// 处理自动对焦参数
		List<String> focusModes = params.getSupportedFocusModes();

		String CAF_PICTURE = Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE, CAF_VIDEO = Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO, supportedMode = focusModes
				.contains(CAF_PICTURE) ? CAF_PICTURE : focusModes
				.contains(CAF_VIDEO) ? CAF_VIDEO : "";

		if (!supportedMode.equals("")) {
			params.setFocusMode(supportedMode);
		}

		camera.setParameters(params);

		try {
			camera.setPreviewDisplay(holder);
		} catch (IOException e) {
			throw new RuntimeException(e);
		}

		camera.startPreview();

		detector = new Detector(this, this);
		detector.start();
		detector.setSize(CAMERA_PREVIEW_WIDTH, CAMERA_PREVIEW_HEIGHT);

		Log.d(TAG, "surface changed.");
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		camera = Camera.open(CAMERA_ID);
		timer = new Timer();
		timer.schedule(new FrameTimerTask(), DELAY_START_FRAME_SAMPLE,
				FRAME_SAMPLE_INTERVAL);
		Log.d(TAG, "surface created.");

	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		timer.cancel();
		detector.stop();

		camera.setPreviewCallback(null);
		camera.stopPreview();
		camera.release();
		Log.d(TAG, "surface destroyed.");

		currentFrame = null;
	}

	class FrameTimerTask extends TimerTask {
		@Override
		public void run() {
			// Log.d(TAG, "frame timer task run .." + camera);
			setShotPreview();
		}
	}

	private void setShotPreview() {
		camera.setOneShotPreviewCallback(this);
	}

	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		currentFrame = data;
	}

	@Override
	public byte[] getCurrentFrame() {
		return currentFrame;
	}

	@Override
	public void onMoveDetected() {
		Log.d(TAG, "detect move, view refresh.");
	}

	@Override
	public void onMatchDetected(int detectIndex) {
		Log.d(TAG, "detect match, index: " + detectIndex);
	}

	@Override
	public Iterator getMatchImages() {
		return null;
	}
}