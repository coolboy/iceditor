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
	 * @throws Exception 
	 */
	public byte[] diff(byte[] originData, byte[] newData) throws Exception;
	
	/**
	 * @param originData
	 * @param diffData
	 * @return newData
	 * @throws Exception 
	 */
	public byte[] merge(byte[] originData, byte[] diffData) throws Exception;

}
