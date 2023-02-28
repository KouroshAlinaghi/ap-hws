#include <iostream>

using namespace std;

int number_of_possible_paths(int size, int x = 0, int y = 0) {
    if (x == size && y == size) 
        return 1;

    if (x > size || y > size || x > y) 
        return 0;

    return number_of_possible_paths(size, x+1, y) + number_of_possible_paths(size, x, y+1);
}

int main() {
    int n;
    cin >> n;
    cout << number_of_possible_paths(n);
    return 0;
}
