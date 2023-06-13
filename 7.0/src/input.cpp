#include <iostream>
#include <vector>

#include "input.hpp"

using namespace std;

string trim(string str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos)
        return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

vector<string> split(string str, char delim) {
    vector<string> res;
    if (str.empty()) return res;
    if (str.back() == 13) str.pop_back();
    string cur;
    for (char c : str) 
        if ((delim == ',' or cur.size()) and (c == delim or c == '\n'))
            res.push_back(trim(cur)), cur = "";
        else
            cur += c;

    if (cur.size()) res.push_back(trim(cur));
    return res;
}
