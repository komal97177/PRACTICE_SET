#include <iostream>
using namespace std;

long long power(long long x, long long y) {
    // Base case
    if (y == 0)
        return 1;

    long long temp = power(x, y / 2);
 // Recursive case
    if (y % 2 == 0)
        return temp * temp;
    else
        return x * temp * temp;
}

int main() {
    long long x = 2, y = 10;
    // Example usage
    cout << power(x, y);
    return 0;
}