package edu.pitt.cs.android;

import android.app.Activity;
//import org.openintents.*;
import org.openintents.distribution.EulaActivity;

import android.os.Bundle;
import android.widget.EditText;

public class HelloAndroid extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //
        if (!EulaActivity.checkEula(this)) {
            return;
         }
        
        setContentView(R.layout.main);
        
        //
        EditText fld = (EditText) findViewById(R.id.field);
        fld.setText("Hello\n World");
    }
}