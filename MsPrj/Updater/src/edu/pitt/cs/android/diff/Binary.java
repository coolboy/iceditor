/**
 * 
 */
package edu.pitt.cs.android.diff;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;

import com.nothome.delta.Delta;
import com.nothome.delta.DiffWriter;
import com.nothome.delta.GDiffPatcher;
import com.nothome.delta.GDiffWriter;

/**
 * @author Cool
 *
 */
public class Binary implements IDiff {

	/* (non-Javadoc)
	 * @see edu.pitt.cs.android.diff.IDiff#diff(byte[], byte[])
	 */
	public byte[] diff(byte[] originData, byte[] newData) throws Exception {
		
		ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        DiffWriter diffWriter = new GDiffWriter(new DataOutputStream(outputStream));
		Delta delta = new Delta();
		delta.compute(originData, new ByteArrayInputStream(newData), diffWriter);
        diffWriter.close();
            
        return outputStream.toByteArray();
	}

	/* (non-Javadoc)
	 * @see edu.pitt.cs.android.diff.IDiff#merge(byte[], byte[])
	 */
	public byte[] merge(byte[] originData, byte[] diffData) throws Exception {
		ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
		GDiffPatcher diffPatcher = new GDiffPatcher();
		diffPatcher.patch(originData, new ByteArrayInputStream(diffData), outputStream);
		
        return outputStream.toByteArray();
	}

}
