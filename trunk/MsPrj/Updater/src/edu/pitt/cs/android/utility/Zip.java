/**
 * 
 */
package edu.pitt.cs.android.utility;

/**
 * @author Cool
 *
 */
import java.io.*;
import java.util.*;
import java.util.zip.*;

public class Zip {

	private static final int BUFFER_SIZE = 2048;

	public static final void copyInputStream(InputStream in, OutputStream out)
			throws IOException {
		byte[] buffer = new byte[1024];
		int len;

		while ((len = in.read(buffer)) >= 0)
			out.write(buffer, 0, len);

		in.close();
		out.close();
	}

	/**
	 * @param zipFilePath
	 * @param outFolder
	 */
	public static void extract(String zipFilePath, File outFolder) {
		try {
			ZipFile zipFile = new ZipFile(zipFilePath);

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
					
					File dir = new File(outFolder, fullDir);
					dir.mkdirs();
					
					File file = new File(dir, fileName);
					file.createNewFile();
					
					copyInputStream(zipFile.getInputStream(entry),
							new BufferedOutputStream(new FileOutputStream(file)));
				}else{//file in root
					String fileName = fullPath;
					
					System.out.println("File: " + fileName);
					
					File file = new File(outFolder, fileName);
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
	
	/**
	 * @param zipFilePath
	 * @param entryPath the file path inside the zip file that you want it to be extracted
	 * @param outPath
	 */
	public static void extract(String zipFilePath, String entryPath, File outPath){
		try {
			ZipFile zipFile = new ZipFile(zipFilePath);

			Enumeration<? extends ZipEntry> entries = zipFile.entries();

			while (entries.hasMoreElements()) {
				ZipEntry entry = entries.nextElement();

				String fullPath = entry.getName();
				if (fullPath != entryPath)
					continue;
				
				System.out.println(fullPath);

				outPath.createNewFile();
					
				copyInputStream(zipFile.getInputStream(entry),
						new BufferedOutputStream(new FileOutputStream(outPath)));
			}

			zipFile.close();
		} catch (IOException ioe) {
			System.out.println("Unhandled exception:");
			ioe.printStackTrace();
		}	
	}
	
	public static byte[] readAll(ZipFile zFile, String Path) throws IOException{
		ZipEntry zEntry = zFile.getEntry(Path);
		
		byte[] entryBinary = new byte[(int) zEntry.getSize()];
		
		BufferedInputStream originIS = new BufferedInputStream(zFile.getInputStream(zEntry), BUFFER_SIZE);
		originIS.read(entryBinary, 0, entryBinary.length);
		
		return entryBinary;
	}
}
