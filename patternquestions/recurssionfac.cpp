#include <iostream>
using namespace std;

long long factorial(int n) {
    // Base case
    if (n == 0 || n == 1)
        return 1;

    // Recursive case
    return n * factorial(n - 1);
}

int main() {
    int n;
    cout << "Enter a non-negative integer: ";
    cin >> n;

    cout << "Factorial of " << n << " is " << factorial(n) << endl;

    return 0;
}