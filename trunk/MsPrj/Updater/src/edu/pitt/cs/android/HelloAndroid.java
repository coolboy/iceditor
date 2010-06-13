package edu.pitt.cs.android;

import android.app.Activity;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class HelloAndroid extends Activity {
	
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

        Button buttonFileManager = (Button) findViewById(R.id.file_manager);
        buttonFileManager.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View arg0) {
				openFile();
			}
        });
        
        Button button = (Button) findViewById(R.id.open);
        button.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View arg0) {
				openFile();
			}
        });

        button = (Button) findViewById(R.id.save);
        button.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View arg0) {
				saveFile();
			}
        });

        button = (Button) findViewById(R.id.pick_directory);
        button.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View arg0) {
				pickDirectory();
			}
        });
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
		intent.putExtra("org.openintents.extra.TITLE", getString(R.string.open_title));
		intent.putExtra("org.openintents.extra.BUTTON_TEXT", getString(R.string.open_button));
		
		try {
			startActivityForResult(intent, REQUEST_CODE_PICK_FILE_OR_DIRECTORY);
		} catch (ActivityNotFoundException e) {
			// No compatible file manager was found.
			Toast.makeText(this, R.string.no_filemanager_installed, 
					Toast.LENGTH_SHORT).show();
		}
	}

    /**
     * Opens the file manager to select a location for saving a file.
     */
    private void saveFile() {
		String fileName = mFile1.getText().toString();
		
		Intent intent = new Intent("org.openintents.action.PICK_FILE");
		
		// Construct URI from file name.
		intent.setData(Uri.parse("file://" + fileName));
		
		// Set fancy title and button (optional)
		intent.putExtra("org.openintents.extra.TITLE", getString(R.string.save_title));
		intent.putExtra("org.openintents.extra.BUTTON_TEXT", getString(R.string.save_button));
		
		try {
			startActivityForResult(intent, REQUEST_CODE_PICK_FILE_OR_DIRECTORY);
		} catch (ActivityNotFoundException e) {
			// No compatible file manager was found.
			Toast.makeText(this, R.string.no_filemanager_installed, 
					Toast.LENGTH_SHORT).show();
		}
	}

    /**
     * Opens the file manager to pick a directory.
     */
    private void pickDirectory() {
		String fileName = mFile1.getText().toString();
		
		// Note the different intent: PICK_DIRECTORY
		Intent intent = new Intent("org.openintents.action.PICK_DIRECTORY");
		
		// Construct URI from file name.
		intent.setData(Uri.parse("file://" + fileName));
		
		// Set fancy title and button (optional)
		intent.putExtra("org.openintents.extra.TITLE", getString(R.string.pick_directory_title));
		intent.putExtra("org.openintents.extra.BUTTON_TEXT", getString(R.string.pick_directory_button));
		
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
					
					mFile1.setText(filename);
				}				
				
			}
			break;
		}
	}
}