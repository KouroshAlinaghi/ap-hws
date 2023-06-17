#include "string"
#include "iostream"

using namespace std;

int main() {
    int n = 0;
    cin >> n;
    while (n) {
        int p = 0, q = 0, m = 0, is_valid = 1, sum = 0;
        cin >> p;
        cin >> q;
        cin >> m;
        for (int i=0; i<m; i++) {
            int start, end;
            cin >> start;
            cin >> end;
            sum += end - start;
            if (sum > p || end - start > q) {
                is_valid = 0;
                break;
            }
        }
        cout << is_valid;
        n--;
    }

    return 0;
}
