package edu.pitt.cs.android;

import java.io.*;

import edu.pitt.cs.android.diff.DiffFactory;
import edu.pitt.cs.android.utility.FileOperation;

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
     * generate diff update for future use
     */
	private void generateDiffFile() {
		Toast.makeText(this, mFile1.getText(), Toast.LENGTH_SHORT).show();
		
		File root = Environment.getExternalStorageDirectory();
		if (!root.canWrite())
			return;
		
		String toastMsg = null;
		
		try {
			Object diff = DiffFactory.getDiff(mFile1.getText().toString(), mFile2.getText().toString());
			
			File diffFile = new File(mFileOut.getText().toString());
			diffFile.createNewFile();
			
			ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(diffFile));
			out.writeObject(diff);
			out.flush();
			
			toastMsg = "Diff created: " + diffFile + " Size: " + diffFile.length();

		} catch (IOException e) {
			toastMsg = e.getMessage();
		}
		
		Toast.makeText(this, toastMsg, Toast.LENGTH_SHORT).show();	
	}

	/**
     *mfile1 src file
     *mfile2 diff file
	 *mfileout des file
     */
	private void mergeDiffFile() {
		String toastMsg = null;
		
		try {
			File of = DiffFactory.createFromDiff(mFile1.getText().toString(), mFile2.getText().toString(), mFileOut.getText().toString());
			toastMsg = "Out file: " + of.getName() + "Length: " + of.length();
		} catch (Throwable e) {
			toastMsg = "Exception: " + e.getMessage();
		}

		Toast.makeText(this, toastMsg, Toast.LENGTH_SHORT).show();
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
	
	/*! Recursively delete a directory and all of its children.
	 *  @params toastOnError If set to true, this function will toast if an error occurs.
	 *  @returns true if successful, false otherwise.
	 */
	private boolean recursiveDelete(File file, boolean toastOnError) {
		// Recursively delete all contents.
		File[] files = file.listFiles();
		
		for (int x=0; x<files.length; x++) {
			File childFile = files[x];
			if (childFile.isDirectory()) {
				if (!recursiveDelete(childFile, toastOnError)) {
					return false;
				}
			} else {
				if (!childFile.delete()) {
					Toast.makeText(this, getString(R.string.error_deleting_child_file, childFile.getAbsolutePath()), Toast.LENGTH_LONG);
					return false;
				}
			}
		}
		
		if (!file.delete()) {
			Toast.makeText(this, getString(R.string.error_deleting_folder, file.getAbsolutePath()), Toast.LENGTH_LONG);
			return false;
		}
		
		return true;
	}

	private void deleteFileOrFolder(File file) {
		
		if (file.isDirectory()) {
			if (recursiveDelete(file, true)) {
				Toast.makeText(this, R.string.folder_deleted, Toast.LENGTH_SHORT).show();
			}
		} else {
			if (file.delete()) {
				// Delete was successful.
				Toast.makeText(this, R.string.file_deleted, Toast.LENGTH_SHORT).show();
			} else {
				Toast.makeText(this, R.string.error_deleting_file, Toast.LENGTH_SHORT).show();
			}
		}
	}
}