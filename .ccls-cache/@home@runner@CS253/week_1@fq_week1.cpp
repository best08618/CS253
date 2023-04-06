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

void read_stopword(string file_path, string *stopwords) {
  ifstream stopwords_file;
  stopwords_file.open(file_path);
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

bool in_array(const std::string &value, string *array) {
  bool check = std::find(array, array + 150, value) != array + 150;
  return check;
}

map<string, int> read_file_count(string file_path, string *stopwords) {
  map<string, int> fq;
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
        if (!in_array(tok, stopwords)) {
          if (fq.find(tok) == fq.end()) {
            fq.insert(make_pair(tok, 1));
            tok = "";
          } else {
            fq[tok]++;
            tok = "";
          }
        }
        filestring.erase(filestring.begin(), filestring.begin() + end + 1);
        end = filestring.find(' ');
      }
      string tok = filestring.substr(0, end);
      if (tok.length() > 1 && !in_array(tok, stopwords)) {
        fq[tok]++;
      }
    }
  }
  return fq;
}
bool comp(const pair<string, int> &a, const pair<string, int> &b) {
  return (a.second > b.second);
}

void sort(map<string, int> fq) {
  vector<pair<string, int>> vec;
  map<string, int>::iterator it;

  for (it = fq.begin(); it != fq.end(); it++) {
    vec.push_back(make_pair(it->first, it->second));
  }

  sort(vec.begin(), vec.end(), comp);
  for (int i = 0; i < 25; ++i) {
    cout << vec[i].first << "  -  " << vec[i].second << endl;
  }
}

int main() {
  string stop_file_path = "../stop_words.txt";
  string file_path = "../pride-and-prejudice.txt";
  // string file_path = "../test.txt";
  string stopwords[150];
  read_stopword(stop_file_path, stopwords);
  map<string, int> fq = read_file_count(file_path, stopwords);
  sort(fq);
}