/**
 * 
 */
package edu.pitt.cs.android.diff;

import com.nothome.delta.text.Delta;
//import com.nothome.delta.text.GDiffTextWriter;
//import com.nothome.delta.text.TextPatcher;;

/**
 * @author Cool
 *
 */
public class Text implements IDiff {

	/* (non-Javadoc)
	 * @see edu.pitt.cs.android.diff.IDiff#diff(byte[], byte[])
	 */
	public byte[] diff(byte[] originData, byte[] newData) throws Exception {
//		ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
		CharSequence srcSeq = originData.toString();
		CharSequence newSeq = newData.toString();
				
		Delta delta = new Delta();
            
        return delta.compute(srcSeq, newSeq).getBytes();
	}

	/* (non-Javadoc)
	 * @see edu.pitt.cs.android.diff.IDiff#merge(byte[], byte[])
	 */
	public byte[] merge(byte[] originData, byte[] diffData) {
		// TODO Auto-generated method stub
		return null;
	}

}
