#include <iostream>
#include <string>

using namespace std;

string to_upper(string str) {
    if (str == "")
        return "";

    return (char)toupper(str[0]) + to_upper(str.substr(1));
}

int main() {
    string line;
    while (getline(cin, line)) {
        cout << to_upper(line) << endl;
    }

    return 0;
}
