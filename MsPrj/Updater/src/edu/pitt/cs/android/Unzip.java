/**
 * 
 */
package edu.pitt.cs.android;

/**
 * @author Cool
 *
 */
import java.io.*;
import java.util.*;
import java.util.zip.*;

public class Unzip {

	public static final void copyInputStream(InputStream in, OutputStream out)
			throws IOException {
		byte[] buffer = new byte[1024];
		int len;

		while ((len = in.read(buffer)) >= 0)
			out.write(buffer, 0, len);

		in.close();
		out.close();
	}

	public static void extract(String zipFileName, File root) {
		Enumeration<? extends ZipEntry> entries;
		ZipFile zipFile;

		try {
			zipFile = new ZipFile(zipFileName);

			entries = zipFile.entries();

			while (entries.hasMoreElements()) {
				ZipEntry entry = entries.nextElement();

				String fullPath = entry.getName();
				System.err.println("Extracting file: " + fullPath);
				File outf = new File(root, entry.getName());
				outf.mkdirs();
				
//				outf.createNewFile();
				
//				copyInputStream(zipFile.getInputStream(entry),
//						new BufferedOutputStream(new FileOutputStream(outf)));
			}

			zipFile.close();
		} catch (IOException ioe) {
			System.err.println("Unhandled exception:");
			ioe.printStackTrace();
		}
	}
}
