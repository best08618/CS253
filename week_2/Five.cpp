#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// shared mutable data
string stopwords[150];
vector<string> words;
map<string, int> fq;

void read_stopword() {
  string stop_file_path = "../stop_words.txt";
  ifstream stopwords_file;
  stopwords_file.open(stop_file_path);
  int i = 0;
  string mystring;
  if (stopwords_file.is_open()) {
    stopwords_file >> mystring;
    string token;
    size_t pos = 0;
    string delimiter = ",";
    while ((pos = mystring.find(delimiter)) != string::npos) {
      token = mystring.substr(0, pos);
      stopwords[i] = token;
      i++;
      mystring.erase(0, pos + delimiter.length());
    }
    if (mystring.length() > 0) {
      stopwords[i] = mystring;
    }

  } else {
    cout << "File open error";
  }
}

bool in_array(string value) {
  bool check = std::find(stopwords, stopwords + 150, value) != stopwords + 150;
  return check;
}

void read_file(string file_path) {
  ifstream file;
  file.open(file_path);
  string filestring;
  if (file.is_open()) {
    while (file.good()) {
      file >> filestring;
      transform(filestring.begin(), filestring.end(), filestring.begin(),
                ::tolower);
      filestring =
          regex_replace(filestring, std::regex(R"([^A-Za-z0-9])"), " ");
      int end = filestring.find(' ');
      while (end != -1) { // Loop until no delimiter is left in the string.
        string tok = filestring.substr(0, end);
        words.push_back(tok);
        filestring.erase(filestring.begin(), filestring.begin() + end + 1);
        end = filestring.find(' ');
      }
      string tok = filestring.substr(0, end);
      if (tok.length() > 1) {
        words.push_back(tok);
      }
    }
  }
}

void frequencies() {
  for (int i = 0; i < words.size(); i++) {
    if (!in_array(words[i])) {
      if (fq.find(words[i]) == fq.end()) {
        fq[words[i]] = 1;
      } else {
        fq[words[i]]++;
      }
    }
  }
}

bool comp(const pair<string, int> &a, const pair<string, int> &b) {
  return (a.second > b.second);
}

void sort() {
  vector<pair<string, int>> vec;
  map<string, int>::iterator it;

  for (it = fq.begin(); it != fq.end(); it++) {
    vec.push_back(make_pair(it->first, it->second));
  }

  sort(vec.begin(), vec.end(), comp);
  int s = fq.size();
  for (int i = 0; i < min(25, s); ++i) {
    cout << vec[i].first << "  -  " << vec[i].second << endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cout << "Please enter the file_path";
    return 0;
  }
  string file_path = argv[1];
  read_stopword();
  read_file(file_path);
  frequencies();
  sort();
}
