/**
 * 
 */
package edu.pitt.cs.apk;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import com.nothome.delta.Delta;
import com.nothome.delta.DiffWriter;
import com.nothome.delta.GDiffWriter;

import edu.pitt.cs.dex.DexDelta;
import edu.pitt.cs.util.FileUtil;
import edu.pitt.cs.util.MD5Checksum;
import edu.pitt.cs.util.UnZip;
import edu.pitt.cs.util.Zipper;

/**
 * @author Cool
 * 
 */
public class ApkDelta {
	private static String baseOutputPath = FileUtil.createTempDirectory().getAbsolutePath();
	private static String srcOutputPath = baseOutputPath + "\\src";
	private static String targetOutputPath = baseOutputPath + "\\target";
	private static String deltaOutputPath = baseOutputPath + "\\delta";
	private static String targetSignatureOutputPath = targetOutputPath + "\\META-INF";

	public static void computeDelta(String sourcePath, String targetPath,
			String deltaOutputFilePath) throws Exception {
		UnZip uz = new UnZip();

		// Decompress src.apk to a folder
		uz.unZip(sourcePath, srcOutputPath);
		// Decompress target.apk to a folder
		uz.unZip(targetPath, targetOutputPath);
		// Delete the signature
		FileUtil.deleteDir(new File(targetSignatureOutputPath));
		// Apply xdelta on the two folders and save the result to a folder
		computeFolderDelta(srcOutputPath, targetOutputPath, deltaOutputPath);
		// compress the deltaOutputFolder to output stream
		Zipper.zipFolder(deltaOutputPath, deltaOutputFilePath);

		// delete the temp folders
		FileUtil.deleteDir(new File(baseOutputPath));
	}

	public static byte[] computeDelta(String sourcePath, String targetPath)
			throws Exception {
		UnZip uz = new UnZip();

		// Decompile source.dex to a folder
		uz.unZip(sourcePath, srcOutputPath);
		// Decompile target.dex to a folder
		uz.unZip(targetPath, targetOutputPath);
		// Delete the signature
		FileUtil.deleteDir(new File(targetSignatureOutputPath));
		// Apply xdelta on the two folders and save the result to a folder
		computeFolderDelta(srcOutputPath, targetOutputPath, deltaOutputPath);
		// compress the deltaOutputFolder to output stream
		byte[] ret = Zipper.zipFolder(deltaOutputPath);

		// delete the temp folders
		FileUtil.deleteDir(new File(baseOutputPath));

		return ret;
	}

	private static void IteratingDir(File srcDir, File targetDir, File deltaDir)
			throws IOException {
		if (srcDir.isDirectory()) {
			String[] children = srcDir.list();
			for (int i = 0; i < children.length; i++) {
				IteratingDir(new File(srcDir, children[i]), targetDir, deltaDir);
			}
		} else {
			File srcFile = srcDir;
			String srcFileRelPath = srcFile.getPath().substring(0 + srcOutputPath.length()); 
			File targetFile = new File(targetDir.getPath()
					+ srcFileRelPath);
			// src has the file but the target doesn't
			// make a place holder in the delta folder with pattern
			// %name%.delete
			if (targetFile.exists() == false ) {
				File mark2DeleteFile = new File(deltaDir.getPath()
						+ srcFileRelPath + ".delete");
				mark2DeleteFile.mkdirs();
				mark2DeleteFile.delete();
				mark2DeleteFile.createNewFile();
			} else {
				try {
					// src and target both have this file
					// save the file delta with pattern %name%.diff to delta
					// folder
					String srcMD5 = MD5Checksum.getMD5Checksum(srcFile
							.getAbsolutePath());
					String targetMD5 = MD5Checksum.getMD5Checksum(targetFile
							.getAbsolutePath());

					if (srcMD5.equals(targetMD5))
						return;// do nothing if they are the same

					String deltaFilePath = deltaDir.getPath()
							+ srcFile.getPath().substring(
									0 + srcOutputPath.length()) + ".diff";
					String deltaFolderPath = deltaFilePath.substring(0,
							deltaFilePath.lastIndexOf('\\'));

					File deltaFile = new File(deltaFolderPath);
					deltaFile.mkdirs();

					deltaFile = new File(deltaFilePath);
					deltaFile.createNewFile();

					if (srcFile.getName().endsWith(".dex")) {//for dex file
						byte[] delta = DexDelta.computeDelta(srcFile.getAbsolutePath(), targetFile.getAbsolutePath());
						FileOutputStream fstream = new FileOutputStream(deltaFile);
						fstream.write(delta);
						
						fstream.flush();
						fstream.close();
					} else {//for normal file
						DiffWriter output = new GDiffWriter(
								new FileOutputStream(deltaFile));
						new Delta().compute(srcFile, targetFile, output);
					}
				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					// these files are useless now
					srcFile.delete();
					targetFile.delete();
				}
			}
		}
	}

	private static void computeFolderDelta(String srcFolderPath,
			String targetFolderPath, String deltaFolderPath) throws IOException {
		// create deltaFolder
		File deltaFolder = new File(deltaFolderPath);
		deltaFolder.mkdir();

		// compare src to target
		IteratingDir(new File(srcFolderPath), new File(targetFolderPath),
				new File(deltaFolderPath));

		// copy the rest target files to delta
		// the rest of the file is those files that
		// src doesn't have but target has
		// copy these file from target to delta fully
		FileUtil.copyDirectory(new File(targetFolderPath), new File(
				deltaFolderPath));
	}

	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		// computeDelta("src/main/resources/src.dex",
		// "src/main/resources/target.dex");
		computeDelta("src/main/resources/src.apk",
				"src/main/resources/target.apk",
				"src/main/resources/delta.apk.zip");
	}

}
