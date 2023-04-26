#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Storagemanager {
private:
  vector<string> words;
  ifstream file;
  void _init(string file_path) { this->file.open(file_path); }
  vector<string> _words() {
    string filestring;
    if (this->file.is_open()) {
      while (this->file.good()) {
        this->file >> filestring;
        transform(filestring.begin(), filestring.end(), filestring.begin(),
                  ::tolower);
        filestring =
            regex_replace(filestring, std::regex(R"([^A-Za-z0-9])"), " ");
        int end = filestring.find(' ');
        while (end != -1) { // Loop until no delimiter is left in the string.
          string tok = filestring.substr(0, end);
          if (tok.length() > 1) {
            this->words.push_back(tok);
          }
          filestring.erase(filestring.begin(), filestring.begin() + end + 1);
          end = filestring.find(' ');
        }
        string tok = filestring.substr(0, end);
        if (tok.length() > 1) {
          this->words.push_back(tok);
        }
      }
    }
    return this->words;
  }

public:
  vector<string> dispatch(string *message) {
    if (message[0].compare("init") == 0) {
      this->_init(message[1]);
      return words;
    } else if (message[0].compare("words") == 0) {
      return this->_words();
    } else {
      cout << "message is not defined";
      return words;
    }
  }
};

class Stopwordmanager {
private:
  vector<string> stopwords;
  bool isstop = false;
  void _init() {
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
  bool _is_stop_word(string word) {
    return (std::find(stopwords.begin(), stopwords.end(), word) !=
            stopwords.end());
  }

public:
  bool dispatch(string *message) {
    if (message[0].compare("init") == 0) {
      this->_init();
      return isstop;
    } else if (message[0].compare("is_stop_word") == 0) {
      return this->_is_stop_word(message[1]);
    } else {
      cout << "message is not defined";
      return isstop;
    }
  }
};
class WordFrequencymangaer {
private:
  map<string, int> fq;
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
      cout << vec[i].first << "  -  " << vec[i].second << endl;
    }
  }

public:
  void dispatch(string *message) {
    if (message[0].compare("increment_count") == 0) {
      _increment_count(message[1]);
    } else if (message[0].compare("sorted") == 0) {
      _sorted();
    } else {
      cout << "message is not defined";
    }
  }
};
class WordFrequencyController {
private:
  Storagemanager _stm;
  Stopwordmanager _swm;
  WordFrequencymangaer _wqm;
  void _init(string path_to_file) {
    _stm = Storagemanager();
    _swm = Stopwordmanager();
    string message[2] = {"init", path_to_file};
    _stm.dispatch(message);
    _swm.dispatch(message);
  }
  void _run() {

    string message[2] = {"words", " "};
    vector<string> w;
    w = _stm.dispatch(message);
    message[0] = "is_stop_word";
    for (vector<string>::iterator wp = w.begin(); wp < w.end(); wp++) {
      message[1] = *wp;
      message[0] = "is_stop_word";
      if (!_swm.dispatch(message)) { // if not stopwords
        message[0] = "increment_count";
        _wqm.dispatch(message);
      }
    }
    message[0] = "sorted";
    _wqm.dispatch(message);
  }

public:
  void dispatch(string *message) {
    if (message[0].compare("init") == 0) {
      return this->_init(message[1]);
    } else if (message[0].compare("run") == 0) {
      return this->_run();
    } else
      cout << "message is not defined";
    return;
  }
};

int main(int argc, char *argv[]) {
  string file_path = argv[1];
  WordFrequencyController wfc = WordFrequencyController();
  string message[2] = {"init", file_path};
  wfc.dispatch(message);
  message[0] = "run";
  wfc.dispatch(message);
}