/**
 * 
 */
package edu.pitt.cs.android.diff;

/**
 * @author Cool
 *
 */
public interface IDiff {
	/**
	 * @param originData
	 * @param newData
	 * @return diffData
	 */
	public byte[] diff(byte[] originData, byte[] newData);
	
	/**
	 * @param originData
	 * @param diffData
	 * @return newData
	 */
	public byte[] merge(byte[] originData, byte[] diffData);

}
