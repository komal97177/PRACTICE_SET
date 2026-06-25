#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, l, r;

    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter lower bound (l): ";
    cin >> l;

    cout << "Enter upper bound (r): ";
    cin >> r;

    vector<int> arr(n);

    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int range = r - l + 1;
    vector<int> freq(range, 0);

    // Count frequencies
    for (int x : arr) {
        if (x < l || x > r) {
            cout << "Error: " << x << " is outside the range ["
                 << l << ", " << r << "]\n";
            return 1;
        }
        freq[x - l]++;
    }

    // Print frequencies
    cout << "\nFrequencies:\n";
    for (int i = 0; i < range; i++) {
        if (freq[i] > 0) {
            cout << (i + l) << " -> " << freq[i] << endl;
        }
    }

    // Print sorted array
    cout << "\nSorted array:\n";
    for (int i = 0; i < range; i++) {
        for (int j = 0; j < freq[i]; j++) {
            cout << (i + l) << " ";
        }
    }

    cout << endl;
    return 0;
}