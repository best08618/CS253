import java.util.*;
import java.nio.file.*;
import java.io.*;
import java.util.stream.*;

class TwentyEight {

	public static HashMap<String, Integer> fq = new HashMap<String, Integer>();
  public static int i = 0;

	public static Stream<String> read_file_line(String path) {
		try {
			Stream<String> stream = Files.lines(Paths.get(path));
			return stream;
		} catch (IOException e) {System.out.println(e);}
  		return null;
    }

	public static Stream<String> all_words(String file_path) {
		Stream<String> words_stream = read_file_line(file_path).flatMap(o1 -> Arrays.stream(o1.toLowerCase().split("[\\W_]+")));
		return words_stream;
    }

    public static Stream<String> nonStopWords(String path) throws IOException {
    Set<String> stopWords = new HashSet<String>();
    Scanner f = new Scanner(new File("../stop_words.txt"), "UTF-8");
    try {
        f.useDelimiter(",");
        while (f.hasNext()) {
            stopWords.add(f.next());
        }
    } finally {
        f.close();
    }
		Stream<String> stream = all_words(path).filter(w -> w.length() >=2 && !stopWords.contains(w));
        return stream;
    }

    public static Stream<List<Map.Entry<String, Integer>>> count_and_sort(String path) throws IOException {
		Stream<List<Map.Entry<String, Integer>>> stream = nonStopWords(path).map(w -> {
			fq.put(w, fq.getOrDefault(w, 0) + 1);
			i++;
			if (i % 5000 == 0) {
				return fq.entrySet().stream().sorted((o2, o3) -> o3.getValue() - o2.getValue()).collect(Collectors.toList());} 
      else {
				return null;
			}
		}).filter(s -> s != null);
		return stream;
    }

	public static void main(String[] args) {
		try {
			 count_and_sort(args[0]).forEach(s -> {
				System.out.println("-----------------------------");
				s.stream().limit(25).forEach(fq -> {
					System.out.println(fq.getKey() + "  -  " + fq.getValue());
				});
			});
		} catch (Exception e) {
      System.out.println(e);
		}
	}

	
}