#include <algorithm>
#include <array>
#include <atomic>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

queue<string> word_space;
queue<map<string, int>> freq_space;
vector<string> stopwords;

void load_stop_word() {
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
      stopwords.push_back(token);
      mystring.erase(0, pos + delimiter.length());
    }
    if (mystring.length() > 0) {
      stopwords.push_back(mystring);
    }

  } else {
    cout << "File open error";
  }
}

void load_data(string file_path) {
  string filestring;
  ifstream file;
  file.open(file_path);
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
        if (tok.length() > 1) {
          word_space.push(tok);
        }
        filestring.erase(filestring.begin(), filestring.begin() + end + 1);
        end = filestring.find(' ');
      }
      string tok = filestring.substr(0, end);
      if (tok.length() > 1) {
        word_space.push(tok);
      }
    }
  }
}

void process_word() {
  map<string, int> word_freq;
  while (true) {
    if (!word_space.empty()) {
      string word = word_space.front();
      word_space.pop();
      if (std::find(stopwords.begin(), stopwords.end(), word) ==
          stopwords.end()) {
        word_freq[word]++;
      }
    } else
      break;
  }
  freq_space.push(word_freq);
}

int main(int argc, char *argv[]) {
  string file_path = argv[1];
  load_stop_word();
  load_data(file_path);
  vector<thread> a;

  for (int i = 0; i < 5; i++) {
    a.emplace_back(process_word);
    a.back().join();
  }
  cout << "counting" << endl << flush;
  map<string, int> word_freq;
  while (!freq_space.empty()) {
    map<string, int> freqs = freq_space.front();
    freq_space.pop();
    for (auto it : freqs) {
      word_freq[it.first] += it.second;
    }
  }
  vector<pair<string, int>> vec;
  map<string, int>::iterator it;

  for (it = word_freq.begin(); it != word_freq.end(); it++) {
    vec.push_back(make_pair(it->first, it->second));
  }
  sort(vec.begin(), vec.end(),
       [](const pair<string, int> &a, const pair<string, int> &b) {
         return (a.second > b.second);
       });
  int s = word_freq.size();
  for (int i = 0; i < min(25, s); ++i) {
    cout << vec[i].first << "  -  " << vec[i].second << endl;
  }

  return 0;
}