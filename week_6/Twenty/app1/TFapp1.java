import java.io.*;
import java.util.*;
import java.lang.reflect.*;
import java.lang.Class;

public class TFapp1 implements TF {
  public List<String> words;
  public Set<String> stopWords;
  public Map<String, Integer> wordFreqs;
  public List<Map.Entry<String, Integer>> entryList;
  public TFapp1() {}
  public void tfOperation(String file_path) {
    try{
      // read file 
        read_file(file_path);
        this.readStopwords();
        this.wordFreqs = new HashMap<String, Integer>();
        for (String word : this.words) {
          if (! this.isStopWord(word)) {
                this.incrementCount(word);
          }
        }
        this.entryList = new LinkedList<>(this.wordFreqs.entrySet());
        boolean order = false;
        this.entryList.sort((o1, o2) -> order ? o1.getValue().compareTo(o2.getValue()) == 0
                ? o1.getKey().compareTo(o2.getKey())
                : o1.getValue().compareTo(o2.getValue()) : o2.getValue().compareTo(o1.getValue()) == 0
                ? o2.getKey().compareTo(o1.getKey())
                : o2.getValue().compareTo(o1.getValue()));
        int numWordsPrinted = 0;
        for (Map.Entry<String, Integer> entry : entryList) {
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
    
    // Add single-letter words
    for (char c = 'a'; c <= 'z'; c++) {
        this.stopWords.add("" + c);
    }
  }
  
  public boolean isStopWord(String word){
      return this.stopWords.contains(word);
  }

  public void incrementCount(String word) {
      if (this.wordFreqs.get(word) == null) {
          this.wordFreqs.put(word, 1);
      } else {
          this.wordFreqs.put(word, 1 + this.wordFreqs.get(word));
      }
  }

}