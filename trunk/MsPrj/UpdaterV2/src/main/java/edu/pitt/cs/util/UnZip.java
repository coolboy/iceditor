package edu.pitt.cs.util;

import java.io.*;
import java.util.*;
import java.util.zip.*;

/**
 * UnZip -- print or unzip a JAR or PKZIP file using java.util.zip. Command-line
 * version: extracts files.
 * 
 * @author Ian Darwin, Ian@DarwinSys.com $Id: UnZip.java,v 1.7 2004/03/07
 *         17:40:35 ian Exp $
 */
public class UnZip {
	/** The ZipFile that is used to read an archive */
	protected ZipFile zippy;

	/** The buffer for reading/writing the ZipFile data */
	protected byte[] buf;

	/** Construct an UnZip object. Just allocate the buffer */
	public UnZip() {
		buf = new byte[8092];
	}

	/** Cache of paths we've mkdir()ed. */
	protected SortedSet<String> dirsMade;

	/** For a given Zip file, process each entry. */
	public void unZip(String fileName, String outFolderPath) {

		// create the output folder
		File outFolder = new File(outFolderPath);
		if (outFolder.exists() == false)
			outFolder.mkdirs();
		else
			FileUtil.deleteDir(outFolder);// clean up the folder

		dirsMade = new TreeSet<String>();
		try {
			zippy = new ZipFile(fileName);
			Enumeration<?> all = zippy.entries();
			while (all.hasMoreElements()) {
				getFile((ZipEntry) all.nextElement(), outFolder.getPath()
						+ File.separatorChar);
			}
		} catch (IOException err) {
			System.err.println("IO Error: " + err);
			// return;
		}
	}

	protected boolean warnedMkDir = false;

	static public void getFile(ZipFile zFile, ZipEntry e, String outputPath) {

		String zipName = e.getName();

		// if a directory, just return.
		if (zipName.endsWith("/")) {
			return;
		}

		try {
			File outputFile = new File(outputPath);
			outputFile.mkdirs();
			outputFile.delete();
			outputFile.createNewFile();
			
			FileOutputStream os = new FileOutputStream(outputFile);

			InputStream is = zFile.getInputStream(e);

			byte[] buf = new byte[8092];

			int n = 0;
			while ((n = is.read(buf)) > 0)
				os.write(buf, 0, n);
			is.close();
			os.close();
		} catch (Exception e1) {
			e1.printStackTrace();
		}
	}
	
	static public File getFile(ZipFile zFile, ZipEntry e) {

		String zipName = e.getName();

		File outputFile = null;
		
		// if a directory, just return.
		if (zipName.endsWith("/")) {
			return null;
		}

		try {
			outputFile = File.createTempFile("unzip", "uz");
			outputFile.mkdirs();
			outputFile.delete();
			outputFile.createNewFile();
			
			FileOutputStream os = new FileOutputStream(outputFile);

			InputStream is = zFile.getInputStream(e);

			byte[] buf = new byte[8092];

			int n = 0;
			while ((n = is.read(buf)) > 0)
				os.write(buf, 0, n);
			is.close();
			os.close();
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		return outputFile;
	}

	/**
	 * Process one file from the zip, given its name. Either print the name, or
	 * create the file on disk.
	 */
	protected void getFile(ZipEntry e, String outFolderPath) throws IOException {
		String zipName = e.getName();
		// zip entry is separated by '/'
		if (zipName.startsWith("/")) {
			if (!warnedMkDir)
				System.out.println("Ignoring absolute paths");
			warnedMkDir = true;
			zipName = zipName.substring(1);
		}
		// if a directory, just return. We mkdir for every file,
		// since some widely-used Zip creators don't put out
		// any directory entries, or put them in the wrong place.
		if (zipName.endsWith("/")) {
			return;
		}
		// Else must be a file; open the file for output
		// Get the directory part.
		int ix = zipName.lastIndexOf('/');
		if (ix > 0) {
			String dirName = zipName.substring(0, ix);
			if (!dirsMade.contains(dirName)) {
				File d = new File(outFolderPath + dirName);
				// If it already exists as a dir, don't do anything
				if (!(d.exists() && d.isDirectory())) {
					// Try to create the directory, warn if it fails
					System.out.println("Creating Directory: " + outFolderPath
							+ dirName);
					if (!d.mkdirs()) {
						System.err.println("Warning: unable to mkdir "
								+ dirName);
					}
					dirsMade.add(dirName);
				}
			}
		}
		System.err.println("Creating " + zipName);
		FileOutputStream os = new FileOutputStream(outFolderPath + zipName);
		InputStream is = zippy.getInputStream(e);
		int n = 0;
		while ((n = is.read(buf)) > 0)
			os.write(buf, 0, n);
		is.close();
		os.close();
	}

	/**
	 * Simple main program, construct an UnZipper, process each .ZIP file from
	 * argv[] through that object.
	 */
	public static void main(String[] argv) {
		UnZip uz = new UnZip();

		uz.unZip("src\\main\\resources\\src.apk", "src\\main\\resources\\src");

		System.err.println("All done!");
	}
}