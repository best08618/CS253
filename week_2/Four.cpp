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
int main(int argc, char *argv[]) {
  if (argc == 1) {
    cout << "Please enter the file_path";
    return 0;
  }
  string file_path = argv[1];
  string stop_file_path = "../stop_words.txt";
  vector<string> stopwords;

  // Read stopwords
  ifstream stopwords_file;
  stopwords_file.open(stop_file_path);
  string mystring;
  if (stopwords_file.is_open()) {
    stopwords_file >> mystring;
    string token = "";
    for (int i = 0; i < mystring.length(); i++) {
      if (mystring[i] == ',') {
        stopwords.push_back(token);
        token = "";
      } else {
        token += mystring[i];
      }
    }
    if (token.length() > 0) {
      stopwords.push_back(token);
    }
  }

  // Read file
  ifstream file;
  file.open(file_path);
  vector<pair<string, int>> fq;
  string filestring;
  string token = "";
  if (file.is_open()) {
    while (file.good()) {
      file >> filestring;
      transform(filestring.begin(), filestring.end(), filestring.begin(),
                ::tolower); // lower
      token = "";
      for (int i = 0; i < filestring.length(); i++) {
        if (std::isalnum(static_cast<unsigned char>(filestring[i]))) {
          token += filestring[i];
        } else {
          if (token.length() > 1 && (find(stopwords.begin(), stopwords.end(),
                                          token) == stopwords.end())) {
            vector<pair<string, int>>::iterator ptr = fq.begin();
            for (; ptr < fq.end(); ptr++) {
              if ((*ptr).first == token) {
                ptr->second += 1;
                if (fq.size() > 1) {
                  // reorder
                  for (vector<pair<string, int>>::iterator check = ptr - 1;
                       check >= fq.begin(); check--) {
                    if (ptr->second > check->second) {
                      pair<string, int> imm = *ptr;
                      ptr->first = check->first;
                      ptr->second = check->second;
                      check->first = imm.first;
                      check->second = imm.second;
                      ptr = check;
                    }
                  }
                }
                break;
              }
            }
            if (ptr >= fq.end()) {
              fq.push_back(make_pair(token, 1));
            }
          }
          token = "";
        }
      }
      if (token.length() > 1 && (find(stopwords.begin(), stopwords.end(),
                                      token) == stopwords.end())) {
        vector<pair<string, int>>::iterator ptr = fq.begin();
        for (; ptr < fq.end(); ptr++) {
          if ((*ptr).first == token) {
            ptr->second += 1;
            if (fq.size() > 1) {
              // reorder
              for (vector<pair<string, int>>::iterator check = ptr - 1;
                   check >= fq.begin(); check--) {
                if (ptr->second > check->second) {
                  pair<string, int> imm = *ptr;
                  ptr->first = check->first;
                  ptr->second = check->second;
                  check->first = imm.first;
                  check->second = imm.second;
                  ptr = check;
                }
              }
            }
            break;
          }
        }
        if (ptr >= fq.end()) {
          fq.push_back(make_pair(token, 1));
        }
      }
    }
  }
  for (int i = 0; i < 25; ++i) {
    std::cout << fq[i].first << " " << fq[i].second << endl;
  }
}