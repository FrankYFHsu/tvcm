
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Scanner;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

/*
 * TVCM產生器在節點開始pause time時，並不會印出位置，所以首先需要補齊pause time時的資料。
 * 這個適用在產生區間為一秒時。
 * 
 */

public class TVCMIntegerTraceCombiner {
	public static void main(String[] args) throws IOException {
		String tmpFilePrefix = "Node_merge";
		long SEED = -1l;

		File tracDir = chooseSourceDirectory();

		String numOfFiles = JOptionPane.showInputDialog("Input the number of files");
		if (numOfFiles == null) {
			System.exit(0);
		}

		int num_file = Integer.parseInt(numOfFiles);

		String timeInfo = JOptionPane.showInputDialog(null, "Input start time and end time", "0 864000");
		if (timeInfo == null) {
			System.exit(0);
		}

		String endSimTime = timeInfo.split("\\s")[1];
		String xInfo = JOptionPane.showInputDialog(null, "Input min X and max X values", "0 1500");
		if (xInfo == null) {
			System.exit(0);
		}

		String yInfo = JOptionPane.showInputDialog(null, "Input min Y and max Y values", "0 1500");
		if (yInfo == null) {
			System.exit(0);
		}

		String seedStr = JOptionPane.showInputDialog(null, "Input the seed number you use!?", "1");
		if (seedStr != null) {
			SEED = Long.parseLong(seedStr);
		}

		String firstLine = timeInfo + " " + xInfo + " " + yInfo + " 0 0";

		String traceDirs = tracDir.getAbsolutePath() + File.separator;

		System.out.println("Start seed " + seedStr + " trace generating.");
		long startTime = System.currentTimeMillis();

		try {

			for (int nodeIndex = 0; nodeIndex < num_file; nodeIndex++) {

				String previousX = "";
				String previousY = "";

				int currentSeconds = 0;

				PrintWriter pw = new PrintWriter(new FileWriter(traceDirs + tmpFilePrefix + nodeIndex));

				Scanner sc = new Scanner(new File(traceDirs + "Node" + nodeIndex + ".txt"));

				while (sc.hasNextLine()) {

					String str = sc.nextLine();

					// TVCM預設是用兩個空白隔開，所以用正則表示式 \s+ 來分割
					String[] linesplit = str.split("\\s+");

					int secondsFromTrace = Integer.parseInt(linesplit[0]);

					if (secondsFromTrace == currentSeconds) {
						pw.println(currentSeconds + "  " + nodeIndex + "  " + linesplit[1] + "  " + linesplit[2]);
						currentSeconds++;
						previousX = linesplit[1];
						previousY = linesplit[2];

					} else {

						while (secondsFromTrace >= currentSeconds) {
							pw.println(currentSeconds + "  " + nodeIndex + "  " + previousX + "  " + previousY);
							currentSeconds++;

						}
					}

				}

				pw.flush();
				pw.close();
				sc.close();

				System.out.println("File " + nodeIndex + " done");
			}

			// =======================檔案merge=============================

			// Scanner[] completeTraceSc = new Scanner[num_file];
			BufferedReader[] completeTraceSc2 = new BufferedReader[num_file];

			for (int i = 0; i < num_file; i++) {

				// completeTraceSc[i] = new Scanner(new File(traceDirs
				// + tmpFilePrefix + (i)));
				completeTraceSc2[i] = new BufferedReader(new FileReader(traceDirs + tmpFilePrefix + (i)));

			}

			String outPutTitle = "TVCM_" + xInfo.split(" ")[1] + "m_" + num_file + "N_" + endSimTime + "s_seed_" + SEED;

			File outPutTxtFile = new File(traceDirs + outPutTitle + ".txt");
			PrintWriter mergedFileOuput = new PrintWriter(new BufferedWriter(new FileWriter(outPutTxtFile)));

			System.out.println(firstLine);
			mergedFileOuput.println(firstLine);
			int seconds = 0;
			OuterLoop: while (true) {
				if (seconds % 1000 == 0) {
					System.out.println("seconds=" + seconds);
					long endTime = System.currentTimeMillis();
					long duration = ((endTime - startTime));
					System.out.println("Pass(I) " + duration + " ms.");
				}
				for (int i = 0; i < num_file; i++) {
					String str;
					// if (completeTraceSc[i].hasNextLine()) {
					// String line = completeTraceSc[i].nextLine();
					if ((str = completeTraceSc2[i].readLine()) != null) {
						String line = str;

						// replace double space with single space
						line = line.replaceAll("\\s+", " ");

						mergedFileOuput.println(line);
						// mergedFileOuput.flush();
					} else {
						break OuterLoop;
					}
				}

				seconds++;
			}
			mergedFileOuput.flush();
			mergedFileOuput.close();

			// ==delete tmp files==

			for (int i = 0; i < num_file; i++) {
				completeTraceSc2[i].close();
				Files.delete(Paths.get(traceDirs + tmpFilePrefix + (i)));
			}

			long endTime = System.currentTimeMillis();
			long duration = ((endTime - startTime));
			System.out.println("done, by " + duration + " ms.");

		} catch (FileNotFoundException e) {

			e.printStackTrace();
			System.out.println(e.getMessage());
		}

	}

	protected static File chooseSourceDirectory() {
		File sourceDirectory = null;
		JFileChooser fc = new JFileChooser();
		fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		fc.setVisible(true);
		int returnVal = fc.showOpenDialog(null);
		if (returnVal == JFileChooser.APPROVE_OPTION) {
			sourceDirectory = fc.getSelectedFile();

		} else {
			System.exit(0);
		}

		return sourceDirectory;
	}
}
