#include "string"
#include "iostream"

using namespace std;

int main() {
    int n = 0;
    cin >> n;
    while (n) {
        int p = 0, q = 0, m = 0, is_valid = 1, sum = 0, longest_duration = 0;
        cin >> p;
        cin >> q;
        cin >> m;
        for (int i=0; i<m; i++) {
            int start, end;
            cin >> start;
            cin >> end;
            longest_duration = end - start > longest_duration ? end - start : longest_duration;
            sum += end - start;
            if (sum > p || end - start > q) {
                is_valid = 0;
                break;
            }
        }
        
        int k = sum > p ? sum - p : 0;
        int j = longest_duration > q ? longest_duration - q : 0;
        cout << k << " " << j << "\n";

        n--;
    }

    return 0;
}
