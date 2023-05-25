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

queue<string> stopword_q;
queue<string> stopword_data;

queue<string> storage_q;
queue<string> storage_data;

queue<string> wf_q;
queue<string> wf_data;

class Stopwordmanager {
private:
  vector<string> stopwords;
  bool isstop = false;
  bool stopMe = false;
  void _is_stop_word(string word) {
    if (!(std::find(stopwords.begin(), stopwords.end(), word) !=
          stopwords.end())) {
      wf_q.push("plus");
      wf_data.push(word);
    }
  }
  void _dispatch(string message, string data) {
    if (message.compare("init") == 0) {
      // this->_init();
    } else if (message.compare("filter") == 0) {
      this->_is_stop_word(data);
    } else if (message.compare("endf") == 0) {
      wf_q.push("sorted");
      wf_data.push(" ");
    }
  }

public:
  Stopwordmanager() {
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
  void run() {
    while (!stopMe) {
      if (!stopword_q.empty()) {
        // cout << stopword_q.front();
        string message = stopword_q.front();
        stopword_q.pop();
        if (message.compare("die") == 0) {
          return;
        }
        string data = stopword_data.front();
        stopword_data.pop();
        // cout << message << "-" << data << endl << std::flush;
        _dispatch(message, data);
      }
    }
  }
};

class Storagemanager {

private:
  vector<string> words;
  ifstream file;
  bool stopMe = false;

  void _dispatch(string message, string data) {
    cout << message << endl << flush;
    if (message.compare("words") == 0) {
      // this->_words();
    } else {
      cout << "message is not defined";
    }
  }

public:
  Storagemanager(string file_path) {
    this->file.open(file_path);
    string filestring;
    if (file.is_open()) {
      while (file >> filestring) {
        transform(filestring.begin(), filestring.end(), filestring.begin(),
                  ::tolower);
        filestring =
            regex_replace(filestring, std::regex(R"([^A-Za-z0-9])"), " ");
        int end = filestring.find(' ');
        while (end != -1) { // Loop until no delimiter is left in the string.
          string tok = filestring.substr(0, end);
          if (tok.length() > 1) {
            stopword_q.push("filter");
            stopword_data.push(tok);
          }
          filestring.erase(filestring.begin(), filestring.begin() + end + 1);
          end = filestring.find(' ');
        }
        string tok = filestring.substr(0, end);
        if (tok.length() > 1) {
          stopword_q.push("filter");
          stopword_data.push(tok);
        }
      }
      stopword_q.push("endf");
      stopword_data.push(" ");
      stopword_q.push("die");
      storage_q.push("die");
    }
    file.close();
  }
  void run() {
    while (!stopMe) {
      if (!storage_q.empty()) {
        cout << storage_q.front();
        string message = storage_q.front();
        storage_q.pop();
        if (message.compare("die") == 0) {
          cout << "storage die!" << flush;
          return;
        }
        string data = storage_data.front();
        storage_data.pop();
        cout << message << "-" << data << endl << std::flush;
        _dispatch(message, data);
      }
    }
  }
};

class WordFrequencymangaer {
private:
  map<string, int> fq;
  bool stopMe = false;
  void _increment_count(string word) { fq[word]++; }
  void _sorted() {
    vector<pair<string, int>> vec;
    map<string, int>::iterator it;

    for (it = fq.begin(); it != fq.end(); it++) {
      vec.push_back(make_pair(it->first, it->second));
    }
    sort(vec.begin(), vec.end(),
         [](const pair<string, int> &a, const pair<string, int> &b) {
           return (a.second > b.second);
         });
    int s = fq.size();
    for (int i = 0; i < min(25, s); ++i) {
      cout << vec[i].first << "  -  " << vec[i].second << endl << flush;
    }
    wf_q.push("die");
    wf_data.push(" ");
  }

  void _dispatch(string message, string data) {
    if (message.compare("plus") == 0) {
      _increment_count(data);
    } else if (message.compare("sorted") == 0) {
      _sorted();
    } else {
      cout << "message is not defined";
    }
  }

public:
  void run() {
    while (!stopMe) {
      cout << "" << flush;
      if (!wf_q.empty()) {
        string message = wf_q.front();
        wf_q.pop();
        if (message.compare("die") == 0) {
          cout << "die" << flush;
          return;
        }
        string data = wf_data.front();
        wf_data.pop();
        _dispatch(message, data);
      }
    }
  }
};

int main(int argc, char *argv[]) {
  string file_path = argv[1];
  Stopwordmanager swm = Stopwordmanager();
  Storagemanager sm = Storagemanager(file_path);
  WordFrequencymangaer wfm = WordFrequencymangaer();
  thread thred1(&Stopwordmanager::run, ref(swm));
  thread thred2(&Storagemanager::run, ref(sm));
  thread thred3(&WordFrequencymangaer::run, ref(wfm));
  thred2.join();
  thred1.join();
  thred3.join();
  return 0;
}