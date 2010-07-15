/**
 * 
 */
package edu.pitt.cs.android.utility;

import java.io.*;

/**
 * @author Cool
 * 
 */
public class FileOperation {
	public static void copy(File src, File dst) throws IOException {
		dst.createNewFile();
		
		InputStream in = new FileInputStream(src);
		OutputStream out = new FileOutputStream(dst); // Transfer bytes from in
														// to out
		byte[] buf = new byte[1024];
		int len;
		while ((len = in.read(buf)) > 0) {
			out.write(buf, 0, len);
		}
		in.close();
		out.close();
	}
}
