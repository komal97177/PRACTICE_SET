#include <bits/stdc++.h>
using namespace std;

int main() {
    int a, b;

    cout << "Enter two numbers: ";
    cin >> a >> b;

    // Euclidean Algorithm
    while (b != 0) {
        int remainder = a % b;
        a = b;
        b = remainder;
    }

    cout << "GCD is: " << a;
    return 0;
}
