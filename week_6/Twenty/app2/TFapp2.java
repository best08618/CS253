import java.io.*;
import java.util.*;
import java.lang.reflect.*;
import java.lang.Class;

public class TFapp2 implements TF {
  public List<String> words;
  public Set<String> stopWords;
  public Map<String, Integer> wordFreqs;
  public TFapp2() {}
  public void tfOperation(String file_path) {
    try{
      // read file 
        read_file(file_path);
        this.readStopwords();
        this.wordFreqs = new HashMap<String, Integer>();
        for (String word : this.words) {
          if ((! this.stopWords.contains(word)) && (word.length() >1)) {
                this.incrementCount(word);
          }
        }

      Map<String, Integer> sortedRelevance = new LinkedHashMap<>();
      this.wordFreqs.entrySet()
               .stream()
               .sorted(Map.Entry.<String, Integer> comparingByValue(Comparator.reverseOrder())
                                .thenComparing(Map.Entry.comparingByKey()))
               .forEachOrdered(x -> sortedRelevance.put(x.getKey(), x.getValue()));
      
        int numWordsPrinted = 0;
        for (Map.Entry<String, Integer> entry : sortedRelevance.entrySet()) {
          System.out.println(entry.getKey() + " - " + entry.getValue());
          numWordsPrinted++;
          if (numWordsPrinted >= 25) {
              break;
          }
        }
      }
      catch(Exception e){
        System.out.println(e); 
      }
  }
 public void read_file(String file_path){
    this.words = new ArrayList<String>();
    try{
      Scanner f = new Scanner(new File(file_path), "UTF-8");
      try {
          f.useDelimiter("[\\W_]+");
          while (f.hasNext()) {
              this.words.add(f.next().toLowerCase());
          }
      } finally {
          f.close();
      }
    }catch(Exception e ){
        System.out.println(e);}
  }
  public void readStopwords(){
    this.stopWords = new HashSet<String>();
    try{ 
      Scanner f = new Scanner(new File("../stop_words.txt"), "UTF-8");
      try {
          f.useDelimiter(",");
          while (f.hasNext()) {
              this.stopWords.add(f.next());
          }
      } finally {
          f.close();
      }
    }catch(Exception e ){
        System.out.println(e); 
    }
  }

  public void incrementCount(String word) {
      if (this.wordFreqs.get(word) == null) {
          this.wordFreqs.put(word, 1);
      } else {
          this.wordFreqs.put(word, 1 + this.wordFreqs.get(word));
      }
  }

}