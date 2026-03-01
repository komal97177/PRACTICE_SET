#include <iostream>
using namespace std;

int main() {
    int n, row, column;
    cout << "Enter the value of n: ";
    cin >> n;

    for (row = 1; row <= n; row++) {
        for (column = 1; column <= n; column++) {
            cout << row;
        }
        cout << endl;
    }
    return 0;
}
/*
OUTPUT:

Enter the value of n: 5
11111
22222
33333
44444
55555

*/