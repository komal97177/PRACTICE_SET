#include <iostream>
using namespace std;

int main() {
    int N, K;

    // Input
    cout << "Enter N ";
    cin >> N ;
    cout << "Enter K ";
    cin >> K;

    // Set K-th bit

    int ans = N | (1 << K);

    // Output
    cout << "Number after setting " << K << "-th bit: ";
    cout << ans << endl;

    return 0;
}