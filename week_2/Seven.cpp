#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>
using namespace std;
int main(int argc, char *argv[]) {
  vector<string> data;
  vector<string> stopwords;
  ifstream in("../stop_words.txt");
  string str;
  while (std::getline(in, str, ','))
    stopwords.push_back(str);
  ifstream inFile(argv[1]);
  stringstream strStream;
  strStream << inFile.rdbuf(); // read the file
  string ds = strStream.str(); // str holds the content of the file
  transform(ds.begin(), ds.end(), ds.begin(), ::tolower);
  std::regex rgx("[^a-zA-Z]+");
  std::sregex_token_iterator iter(ds.begin(), ds.end(), rgx, -1), end;
  for (; iter != end; ++iter)
    if (find(stopwords.begin(), stopwords.end(), *iter) == stopwords.end() &&
        (*iter).length() > 1)
      data.push_back(*iter);
  map<string, int> fq;
  for (auto v : data)
    ++fq[v];
  vector<pair<string, int>> vec;
  for (auto x = fq.begin(); x != fq.end(); x++)
    vec.push_back(*x);
  sort(vec.begin(), vec.end(),
       [](pair<string, int> elem1, pair<string, int> elem2) {
         return elem1.second > elem2.second;
       });
  for (vector<pair<string, int>>::iterator x = vec.begin();
       x < vec.begin() + 25; x++) {
    cout << x->first << "  -  " << x->second << endl;
  }
}
