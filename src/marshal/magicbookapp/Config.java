package marshal.magicbookapp;

public final class Config {

	public static final String TAG = "MagicBookApp";

	public static final int CAMERA_ID = 0;

	public static final long DELAY_START_DETECT = 100;

	public static final long DELAY_START_FRAME_SAMPLE = 100;

	public static final long FRAME_SAMPLE_INTERVAL = 50;

	public static final long DELAY_START_DETECT_MOVE = 100;

	public static final long DETECT_MOVE_INTERVAL = 100;

	public static final int CAMERA_PREVIEW_WIDTH = 1920;

	public static final int CAMERA_PREVIEW_HEIGHT = 1080;

	public static final int MAX_DETECT_NOT_MOVE_COUNT = 4;

	private Config() {
	}
}
