/**
 * 
 */
package edu.pitt.cs.apk;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

import com.nothome.delta.GDiffPatcher;

import edu.pitt.cs.dex.DexPatcher;
import edu.pitt.cs.util.FileUtil;
import edu.pitt.cs.util.Stream;
import edu.pitt.cs.util.UnZip;

/**
 * @author Cool
 * 
 */
public class ApkPatcher {
	private static String DELTA_BASE = "delta/";

	public void applyDelta(String apkFilePath, String deltaPath,
			String outputPath) throws IOException {

		ZipFile source = new ZipFile(apkFilePath);
		ZipFile patch = new ZipFile(deltaPath);
		ZipOutputStream output = new ZipOutputStream(new FileOutputStream(
				outputPath));

		try {
			// for each entry in source
			// see if it also exist in the patch

			Enumeration<? extends ZipEntry> srcEntries = source.entries();

			while (srcEntries.hasMoreElements()) {
				ZipEntry se = srcEntries.nextElement();

				String pathInSrc = se.getName();

				// see if delete exist
				String pathInPatch = DELTA_BASE + pathInSrc + ".delete";

				ZipEntry pe = patch.getEntry(pathInPatch);
				if (pe != null)
					continue;

				// see if diff exist
				pathInPatch = DELTA_BASE + pathInSrc + ".diff";
				pe = patch.getEntry(pathInPatch);
				byte[] content = null;
				if (pe != null) {
					// diff!
					boolean dexMode = se.getName().endsWith(".dex");

					File orgFile = UnZip.getFile(source, se);
					File deltaFile = UnZip.getFile(patch, pe);
					File outputFile = File.createTempFile("org", "apk");
					
					if (dexMode) {
						new DexPatcher().applyDelta(orgFile.getAbsolutePath(),
								deltaFile.getAbsolutePath(), outputFile.getAbsolutePath());
					} else {

						GDiffPatcher diffPatcher = new GDiffPatcher();

						diffPatcher.patch(orgFile, deltaFile, outputFile);
					}

					content = FileUtil.toByteArray(outputFile);
					// delete temp files
					orgFile.delete();
					deltaFile.delete();
					outputFile.delete();
				} else {
					// just copy the content
					content = Stream.toByteArray(source.getInputStream(se));
				}

				// write the file to the new output
				ZipEntry outputEntry = new ZipEntry(se.getName());
				output.putNextEntry(outputEntry);
				output.write(content);
				output.closeEntry();
			}

		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			source.close();
			patch.close();
			output.close();
		}
	}

	/**
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {
		new ApkPatcher().applyDelta("src/main/resources/src.apk",
				"src/main/resources/delta.apk.zip",
				"src/main/resources/new.apk");
	}

}
