/**
 * 
 */
package edu.pitt.cs.apk;

/**
 * @author Cool
 *
 */
public class Main {

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		if (args.length != 5) {
			System.err.println("usage apkdiff -m diff sourcePath targetPath deltaOutputPath");
			System.err.println("usage apkdiff -m merge sourcePath deltaPath targetOutputPath");
			return;
		}

		if (args[1].equalsIgnoreCase("diff")){
			ApkDelta.computeDelta(args[2], args[3], args[4]);
		}else if (args[1].equalsIgnoreCase("merge")){
			ApkPatcher.applyDelta(args[2], args[3], args[4]);
		}else
			System.err.println("Wrong mode: " + args[1]);
		
	}

}
