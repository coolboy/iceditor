/**
 * 
 */
package edu.pitt.cs.android.diff;

/**
 * @author Cool
 *
 */
public class Binary implements IDiff {

	/* (non-Javadoc)
	 * @see edu.pitt.cs.android.diff.IDiff#diff(byte[], byte[])
	 */
	public byte[] diff(byte[] originData, byte[] newData) {
		//TODO
		return newData;
	}

	/* (non-Javadoc)
	 * @see edu.pitt.cs.android.diff.IDiff#merge(byte[], byte[])
	 */
	public byte[] merge(byte[] originData, byte[] diffData) {
		// TODO Auto-generated method stub
		return diffData;
	}

}
