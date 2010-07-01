package edu.pitt.cs.android;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

import android.app.Activity;

import android.content.*;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.*;

public class FileSelectionActivity extends Activity {

	protected static final int REQUEST_CODE_PICK_FILE_OR_DIRECTORY = 1;

	protected EditText mFile1;
	protected EditText mFile2;
	protected EditText mFileOut;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		mFile1 = (EditText) findViewById(R.id.file_path1);
		mFile2 = (EditText) findViewById(R.id.file_path2);
		mFileOut = (EditText) findViewById(R.id.file_path_out);

		Button button = (Button) findViewById(R.id.open);
		button.setOnClickListener(new View.OnClickListener() {

			public void onClick(View arg0) {
				openFile();
			}
		});

		// diff!
		button = (Button) findViewById(R.id.diff_button);
		button.setOnClickListener(new View.OnClickListener() {

			public void onClick(View arg0) {
				generateDiffFile();
			}
		});

		// merge!
		button = (Button) findViewById(R.id.merge_button);
		button.setOnClickListener(new View.OnClickListener() {

			public void onClick(View arg0) {
				mergeDiffFile();
			}
		});
	}

	/**
     * 
     */
	private void generateDiffFile() {
		// TODO
		Toast.makeText(this, mFile1.getText(), Toast.LENGTH_SHORT).show();
		// Toast.makeText(this, mFile2.getText(),
		// Toast.LENGTH_SHORT).show();
		// ZipFile zf = null;
		// try {
		// zf = new ZipFile(mFile1.getText().toString());
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		//		
		// Enumeration<? extends ZipEntry> zes = zf.entries();//what this mean?
		// while (zes.hasMoreElements()){
		// ZipEntry ze = zes.nextElement();
		// }
		
		File root = Environment.getExternalStorageDirectory();
		if (!root.canWrite())
			return;
		
		File updaterFolder = new File(root, "updater");
		updaterFolder.mkdir();
		
		//clean up the old dir
		
		Unzip.extract(mFile1.getText().toString(), updaterFolder);
//			File gpxfile = new File(root, "test.txt");
//			gpxfile.createNewFile();
		//this will be used to write the downloaded data into the file we created  
//			FileOutputStream fileOutput = new FileOutputStream(gpxfile);

	}

	/**
     * 
     */
	private void mergeDiffFile() {
		// TODO
		Toast.makeText(this, R.string.todo_msg, Toast.LENGTH_SHORT).show();
	}

	/**
	 * Opens the file manager to select a file to open.
	 */
	private void openFile() {
		String fileName = mFile1.getText().toString();

		Intent intent = new Intent("org.openintents.action.PICK_FILE");

		// Construct URI from file name.
		intent.setData(Uri.parse("file://" + fileName));

		// Set fancy title and button (optional)
		intent.putExtra("org.openintents.extra.TITLE",
				getString(R.string.open_title));
		intent.putExtra("org.openintents.extra.BUTTON_TEXT",
				getString(R.string.open_button));

		try {
			startActivityForResult(intent, REQUEST_CODE_PICK_FILE_OR_DIRECTORY);
		} catch (ActivityNotFoundException e) {
			// No compatible file manager was found.
			Toast.makeText(this, R.string.no_filemanager_installed,
					Toast.LENGTH_SHORT).show();
		}
	}

	/**
	 * This is called after the file manager finished.
	 */
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		switch (requestCode) {
		case REQUEST_CODE_PICK_FILE_OR_DIRECTORY:
			if (resultCode == RESULT_OK && data != null) {
				// obtain the filename
				String filename = data.getDataString();
				if (filename != null) {
					// Get rid of URI prefix:
					if (filename.startsWith("file://")) {
						filename = filename.substring(7);
					}

					((EditText) this.getCurrentFocus()).setText(filename);
				}

			}
			break;
		}
	}
}