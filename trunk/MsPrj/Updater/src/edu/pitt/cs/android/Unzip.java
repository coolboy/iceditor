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
		try {
			ZipFile zipFile = new ZipFile(zipFileName);

			Enumeration<? extends ZipEntry> entries = zipFile.entries();

			while (entries.hasMoreElements()) {
				ZipEntry entry = entries.nextElement();

				String fullPath = entry.getName();
				int folderIndicator = fullPath.lastIndexOf("/");
				if (folderIndicator != -1){//file in folder
					String fullDir = fullPath.substring(0, folderIndicator);
					String fileName = fullPath.substring(folderIndicator + 1);
					
					System.out.println("File: " + fileName);
					System.out.println("Folder: " + fullDir);
					
					File dir = new File(root, fullDir);
					dir.mkdirs();
					
					File file = new File(dir, fileName);
					file.createNewFile();
					
					copyInputStream(zipFile.getInputStream(entry),
							new BufferedOutputStream(new FileOutputStream(file)));
				}else{//file in root
					String fileName = fullPath;
					
					System.out.println("File: " + fileName);
					
					File file = new File(root, fileName);
					file.createNewFile();
					
					copyInputStream(zipFile.getInputStream(entry),
							new BufferedOutputStream(new FileOutputStream(file)));
				}
			}

			zipFile.close();
		} catch (IOException ioe) {
			System.out.println("Unhandled exception:");
			ioe.printStackTrace();
		}
	}
}
