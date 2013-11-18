package marshal.magicbookapp;

import android.app.Activity;
import android.os.Bundle;
import android.view.ViewGroup;
import android.view.WindowManager;

public class MainActivity extends Activity {
	
	private MagicView magicView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		setContentView(R.layout.activity_main);
		
		ViewGroup rootView=(ViewGroup) findViewById(R.id.rootView);
		magicView=new MagicView(this);
		rootView.addView(magicView);
		
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		magicView.resume();
	}

	@Override
	protected void onPause() {
		magicView.pause();
		super.onPause();
	}
}
