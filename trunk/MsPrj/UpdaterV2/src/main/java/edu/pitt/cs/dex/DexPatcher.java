/**
 * 
 */
package edu.pitt.cs.dex;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.Stack;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

import com.nothome.delta.text.TextPatcher;

import edu.pitt.cs.util.FileUtil;
import edu.pitt.cs.util.Stream;
import edu.pitt.cs.util.UnDex;

/**
 * @author Cool
 * 
 */
public class DexPatcher {

	private static final String DEX_BASE = "classes.dex";

	private static String baseOutputPath = FileUtil.createTempDirectory().getAbsolutePath();
	private static String srcOutputPath = baseOutputPath + "\\src";
	private static String dexOutputPath = baseOutputPath + "\\dex";

	public void applyDelta(String dexFilePath, String deltaPath,
			String outputPath) throws IOException {
		ZipFile patch = new ZipFile(deltaPath);
		try {
			// Decompile source.dex to a folder
			UnDex.decompileDex(dexFilePath, srcOutputPath);

			// Create dex output folder
			File dexOutputFolder = new File(dexOutputPath);
			dexOutputFolder.mkdirs();

			// loop through every file
			File baseFolder = new File(srcOutputPath);
			Stack<File> directories = new Stack<File>();
			directories.add(baseFolder);

			while (directories.isEmpty() == false) {
				File dir = directories.pop();
				// get children
				for (String subname : dir.list()) {
					File subfile = new File(dir, subname);
					if (subfile.isDirectory()) {
						directories.add(subfile);
						continue;
					}
					// for each file
					// get the relative path
					String relPath = baseFolder.toURI()
							.relativize(subfile.toURI()).getPath();

					// get the patch entry in patch file
					// patch entry for delete
					ZipEntry pe = patch.getEntry(DEX_BASE + '/' + relPath
							+ ".delete");
					if (pe != null)
						continue; // this file is no longer there any more

					// patch entry for difference
					pe = patch.getEntry(DEX_BASE + '/' + relPath + ".diff");

					String out = null;
					if (pe != null) {// exist but has different content
						String org = FileUtil.toStringBuilder(subfile).toString();
						String delta = Stream.readAll(patch.getInputStream(pe));

						out = new TextPatcher(org).patch(delta);
					} else {
						// can't find the file in the patch, just copy the file
						// to the output
						out = FileUtil.toStringBuilder(subfile).toString();
					}

					// Write the out to the output
					File outputFile = new File(dexOutputFolder, relPath);
					outputFile.mkdirs();
					outputFile.delete();
					outputFile.createNewFile();

					FileOutputStream fo = new FileOutputStream(outputFile);

					OutputStreamWriter ow = new OutputStreamWriter(fo);
					ow.write(out);

					ow.flush();
					ow.close();

					fo.flush();
					fo.close();
				}
			}

			// compile the folder
			String[] args = { new File(dexOutputPath).getAbsolutePath(), "-o",
					outputPath };
			org.jf.smali.main.main(args);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			patch.close();

			// Delete the output folder
			FileUtil.deleteDir(new File(baseOutputPath));
		}
	}

	/**
	 * Main method to make
	 * {@link #applyDelta(ZipFile, ZipFile, ZipOutputStream)} available at the
	 * command line.<br>
	 * usage JarPatcher source patch output
	 */
	public static void main(String[] args) throws IOException {
		new DexPatcher().applyDelta("src/main/resources/src.dex",
				"src/main/resources/delta.zip", "src/main/resources/new.dex");
	}

}
