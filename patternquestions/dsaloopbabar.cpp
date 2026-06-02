#include <iostream>
using namespace std;

int main() {
    int n, i;
    int evenSum = 0, oddSum = 0;

    cout << "Enter the value of n: ";
    cin >> n;

    for (i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            evenSum = evenSum + i;
        } else {
            oddSum = oddSum + i;
        }
    }

    cout << "Sum of even numbers = " << evenSum << endl;
    cout << "Sum of odd numbers = " << oddSum << endl;

    return 0;
}
