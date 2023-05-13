import java.util.*;
import java.io.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;

class Main {
  public static void main(String[] args) {
    try {     
      Properties appProps = new Properties();
      appProps.load(new FileInputStream(args[0]));
      String name = appProps.getProperty("classname");
      String url = appProps.getProperty("pluginpath");
      String file_path = appProps.getProperty("filepath");
  	  System.out.println("Loading and instantiating " + name + "...");
      System.out.println("from path:  " + url);
      Class cls = null;
      URL classUrl = null;
      try {
        classUrl = new URL(url);
      } catch (Exception e) {
        e.printStackTrace();
      }
      URL[] classUrls = {classUrl};
      URLClassLoader cloader = new URLClassLoader(classUrls);
      try {
        cls = cloader.loadClass(name);
      } catch (Exception e) {
        e.printStackTrace();
      } 
  
      if (cls != null) {
  	    try {
      		TF tf = (TF)cls.newInstance();
  		    tf.tfOperation(file_path);
  	    } catch (Exception e) {
  		    e.printStackTrace();
  	    }
  
      }
    }catch(Exception e ){System.out.println(e);}
  }
}