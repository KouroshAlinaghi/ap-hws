#include <iostream>
#include <vector>

using namespace std;

void misaghi(int* arr, int size, int i=0, int counter=0, int last_value=0) {
    if (i >= size)
        return;
    cout << arr[i] << " ";
    int j = counter % 2 == 0 ? max(arr[i], last_value) : min(arr[i], last_value);
    misaghi(arr, size, i+j, counter+1, arr[i]);
}

int main() {
    int n, number;
    cin >> n;
    int arr[n];
    for (int i=0; i<n; i++) {
        cin >> arr[i];
    }

    misaghi(arr, n);
}
