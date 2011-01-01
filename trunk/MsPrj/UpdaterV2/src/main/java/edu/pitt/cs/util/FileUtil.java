/**
 * 
 */
package edu.pitt.cs.util;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.Reader;

/**
 * @author Cool
 * 
 */
public class FileUtil {
	// Deletes all files and sub directories under dir.
	// Returns true if all deletions were successful.
	// If a deletion fails, the method stops attempting to delete and returns
	// false.
	public static boolean deleteDir(File dir) {
		if (dir.isDirectory()) {
			String[] children = dir.list();
			for (int i = 0; i < children.length; i++) {
				boolean success = deleteDir(new File(dir, children[i]));
				if (!success) {
					return false;
				}
			}
		}

		// The directory is now empty so delete it
		return dir.delete();
	}

	// read all from a file to StringBuilder
	public static StringBuilder toStringBuilder(File file) throws IOException {

		InputStreamReader isr = new InputStreamReader(new FileInputStream(file));
		Reader r = new BufferedReader(isr);

		StringBuilder sb = new StringBuilder();
		while (true) {
			int read = r.read();
			if (read == -1)
				break;
			sb.append((char) read);
		}

		isr.close();

		return sb;
	}

	// read all from a file to byte[]
	static public byte[] toByteArray(File file) throws IOException {
		FileInputStream in = new FileInputStream(file);
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		byte[] b = new byte[4096];
		for (int n; (n = in.read(b)) != -1;) {
			out.write(b, 0, n);
		}
		return out.toByteArray();
	}

	public static File createTempDirectory(){
		File temp = null;
		try {
			temp = File.createTempFile("temp", Long.toString(System.nanoTime()));
		} catch (IOException e) {
			e.printStackTrace();
		}

		if (!(temp.delete())) {
			return null;
//			throw new IOException("Could not delete temp file: "
//					+ temp.getAbsolutePath());
		}

		if (!(temp.mkdir())) {
			return null;
//			throw new IOException("Could not create temp directory: "
//					+ temp.getAbsolutePath());
		}

		return (temp);
	}

	// If targetLocation does not exist, it will be created.
	public static void copyDirectory(File sourceLocation, File targetLocation)
			throws IOException {

		if (sourceLocation.isDirectory()) {
			if (!targetLocation.exists()) {
				targetLocation.mkdir();
			}

			String[] children = sourceLocation.list();
			for (int i = 0; i < children.length; i++) {
				copyDirectory(new File(sourceLocation, children[i]), new File(
						targetLocation, children[i]));
			}
		} else {

			InputStream in = new FileInputStream(sourceLocation);
			OutputStream out = new FileOutputStream(targetLocation);

			// Copy the bits from instream to outstream
			byte[] buf = new byte[1024];
			int len;
			while ((len = in.read(buf)) > 0) {
				out.write(buf, 0, len);
			}
			in.close();
			out.close();
		}
	}
}
