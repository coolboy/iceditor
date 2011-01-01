package edu.pitt.cs.util;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.jf.baksmali.baksmali;
import org.jf.baksmali.dump;
import org.jf.dexlib.DexFile;

public class UnDex {
	public static void decompileDex(String dexFileName, String outputFolderName) {
		boolean disassemble = true;
		boolean doDump = false;
		boolean write = false;
		boolean sort = false;
		boolean fixRegisters = false;
		boolean noParameterRegisters = false;
		boolean useLocalsDirective = false;
		boolean useSequentialLabels = false;
		boolean outputDebugInfo = true;
		boolean addCodeOffsets = false;
		boolean deodex = false;
		boolean verify = false;
		boolean ignoreErrors = false;

		int registerInfo = 0;

		String outputDirectory = outputFolderName;
		String dumpFileName = null;
		String outputDexFileName = null;
		String inputDexFileName = dexFileName;
		String bootClassPath = null;
		StringBuffer extraBootClassPathEntries = new StringBuffer();
		List<String> bootClassPathDirs = new ArrayList<String>();
		bootClassPathDirs.add(".");

		try {
			File dexFileFile = new File(inputDexFileName);
			if (!dexFileFile.exists()) {
				System.err.println("Can't find the file " + inputDexFileName);
				System.exit(1);
			}

			// Read in and parse the dex file
			DexFile dexFile = new DexFile(dexFileFile, !fixRegisters, false);

			if (dexFile.isOdex()) {
				if (doDump) {
					System.err
							.println("-D cannot be used with on odex file. Ignoring -D");
				}
				if (write) {
					System.err
							.println("-W cannot be used with an odex file. Ignoring -W");
				}
				if (!deodex) {
					System.err
							.println("Warning: You are disassembling an odex file without deodexing it. You");
					System.err
							.println("won't be able to re-assemble the results unless you deodex it with the -x");
					System.err.println("option");
				}
			} else {
				deodex = false;

				if (bootClassPath == null) {
					bootClassPath = "core.jar:ext.jar:framework.jar:android.policy.jar:services.jar";
				}
			}

			if (disassemble) {
				String[] bootClassPathDirsArray = new String[bootClassPathDirs
						.size()];
				for (int i = 0; i < bootClassPathDirsArray.length; i++) {
					bootClassPathDirsArray[i] = bootClassPathDirs.get(i);
				}

				baksmali.disassembleDexFile(dexFileFile.getPath(), dexFile,
						deodex, outputDirectory, bootClassPathDirsArray,
						bootClassPath, extraBootClassPathEntries.toString(),
						noParameterRegisters, useLocalsDirective,
						useSequentialLabels, outputDebugInfo, addCodeOffsets,
						registerInfo, verify, ignoreErrors);
			}

			if ((doDump || write) && !dexFile.isOdex()) {
				try {
					dump.dump(dexFile, dumpFileName, outputDexFileName, sort);
				} catch (IOException ex) {
					System.err.println("Error occured while writing dump file");
					ex.printStackTrace();
				}
			}
		} catch (RuntimeException ex) {
			System.err.println("\n\nUNEXPECTED TOP-LEVEL EXCEPTION:");
			ex.printStackTrace();
			System.exit(1);
		} catch (Throwable ex) {
			System.err.println("\n\nUNEXPECTED TOP-LEVEL ERROR:");
			ex.printStackTrace();
			System.exit(1);
		}
	}
}
