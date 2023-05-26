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

vector<string> stopwords;
vector<string> filestrings;
queue<map<string, int>> freq_space;
map<string, vector<int>> regroup;
map<string, int> fq;

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
      filestrings.push_back(filestring);
    }
  }
}

void process_word(int i) {
  int chunck_size = int(filestrings.size() / 5) + 1;
  map<string, int> word_freq;
  for (int readpt = (chunck_size * i);
       readpt < ((chunck_size * i) + chunck_size); readpt++) {
    if (readpt >= filestrings.size() - 1)
      break;
    string filestring = filestrings[readpt];
    transform(filestring.begin(), filestring.end(), filestring.begin(),
              ::tolower);
    filestring = regex_replace(filestring, std::regex(R"([^A-Za-z0-9])"), " ");
    int end = filestring.find(' ');
    while (end != -1) { // Loop until no delimiter is left in the string.
      string tok = filestring.substr(0, end);
      if ((tok.length() > 1) &&
          (find(stopwords.begin(), stopwords.end(), tok) == stopwords.end())) {
        word_freq[tok]++;
      }
      filestring.erase(filestring.begin(), filestring.begin() + end + 1);
      end = filestring.find(' ');
    }
    string tok = filestring.substr(0, end);
    if ((tok.length() > 1) &&
        (find(stopwords.begin(), stopwords.end(), tok) == stopwords.end())) {
      word_freq[tok]++;
    }
  }
  freq_space.push(word_freq);
}

void regrouping() {
  while (!freq_space.empty()) {
    map<string, int> fq = freq_space.front();
    freq_space.pop();
    for (auto it : fq) {
      regroup[it.first].push_back(it.second);
    }
  }
}

void count_word(string w) {
  vector<int> cnt = regroup[w];
  int sum = 0;
  for (auto i : cnt) {
    sum += i;
  }
  fq[w] = sum;
}

int main(int argc, char *argv[]) {
  string file_path = argv[1];
  load_stop_word();
  load_data(file_path);
  vector<thread> a;
  for (int i = 0; i < 5; i++) {
    a.emplace_back(process_word, i);
    a.back().join();
  }
  regrouping();
  cout << "regroup" << endl << flush;
  vector<thread> b;
  for (auto g : regroup) {
    string word = g.first;
    b.emplace_back(count_word, word);
    b.back().join();
  }

  vector<pair<string, int>> vec;
  for (auto it = fq.begin(); it != fq.end(); it++) {
    vec.push_back(make_pair(it->first, it->second));
  }
  sort(vec.begin(), vec.end(),
       [](const pair<string, int> &a, const pair<string, int> &b) {
         return (a.second > b.second);
       });
  int s = fq.size();
  for (int i = 0; i < min(25, s); ++i) {
    cout << vec[i].first << "  -  " << vec[i].second << endl;
  }
  return 0;
}