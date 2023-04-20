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

void merge(vector<pair<string, int>> &fq, vector<pair<string, int>> left,
           vector<pair<string, int>> right, int left_size, int right_size) {
  int i = 0, j = 0, k = 0;
  while (i < left_size && j < right_size) {
    if (left[i].second <= right[j].second) {
      fq[k++] = left[i++];
    } else {
      fq[k++] = right[j++];
    }
  }
  while (i < left_size) {
    fq[k++] = left[i++];
  }
  while (j < right_size) {
    fq[k++] = right[j++];
  }
}

void sort(vector<pair<string, int>> &fq, int n) {

  if (n < 2) {
    return;
  }
  int left_size = n / 2;
  int right_size = n - left_size;
  vector<pair<string, int>> leftvector, rightvector;
  for (int i = 0; i < left_size; i++) {
    leftvector.push_back(fq[i]);
  }
  for (int i = left_size; i < n; i++) {
    rightvector.push_back((fq[i]));
  }
  sort(leftvector, left_size); // recursive here!
  sort(rightvector, right_size);
  merge(fq, leftvector, rightvector, left_size, right_size);
}
void print(vector<pair<string, int>> vec) {
  int n = vec.size();
  for (int i = 1; i < 26; i++) {
    cout << vec[n - i].first << " - " << vec[n - i].second << endl;
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

  vector<pair<string, int>> vec;
  vector<pair<string, int>> sorted_vec;
  map<string, int>::iterator it;
  for (it = fq.begin(); it != fq.end(); it++) {
    vec.push_back(make_pair(it->first, it->second));
  }

  sort(vec, vec.size());
  print(vec);
}
