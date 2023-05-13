# Week6

## Seventeen 
```
java Seventeen.java
java Seventeen ../pride-and-prejudice.txt
```
These commands compile the java code and run the codes.
It will show the TF results and prompt asking Class Name. 
If you enter a class name, it will show the information of the class. 

Example) 
```
Enter the Class Name: 
DataStorageManager
Information of Class
Fields==================
private java.util.List DataStorageManager.words
Methods==================
public java.util.List DataStorageManager.getWords()
public java.lang.String DataStorageManager.getInfo()
Superclass==================
class TFExercise
class java.lang.Object
Interfaces==================
```

## Twenty
Implement a prototype of framework and plugin
This has a folder. Please enter in the folder.
0. 
```
cd Twenty
```
1. Framework
```
cd framework
javac *.java
jar cfm framework.jar manifest.mf *.class  # This commandd is for making jar file of framework)
jar cf dependency.jar TF.class  #This commandd is for making jar file of dependency part only
```

2. app1
```
cd app1
cp ../framework/dependency.jar ./   # get dependency jar from framework 
javac -cp ./dependency.jar *.java
jar cf app1.jar *.class
```

3.app2
```
cd app2
cp ../framework/dependency.jar ./   # get dependency jar from framework 
javac -cp ./dependency.jar *.java
jar cf app2.jar *.class
```

4. deploy
```
cd deploy
cp ../framework/framework.jar ./
cp ../app1/app1.jar ./
cp ../app2/app2.jar ./
java -jar framework.jar app.config # Fially, run the framework.jar file with config.
```
You can change the config to point the app that you wan to run. 

Example Config (If you want to run app1 change app2 -> app1 ! )
```
classname=TFapp2
pluginpath=file:////home/runner/CS253/week_6/Twenty/deploy/app2.jar
filepath=../pride-and-prejudice.txt
```

