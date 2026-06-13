#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string recipe;
long long nb, ns, nc;      // Available Bread, Sausage, Cheese
long long pb, ps, pc;      // Prices
long long r;               // Money available

long long cntB = 0, cntS = 0, cntC = 0;

bool canMake(long long burgers) {
    long long needB = max(0LL, cntB * burgers - nb);
    long long needS = max(0LL, cntS * burgers - ns);
    long long needC = max(0LL, cntC * burgers - nc);

    long long cost = needB * pb + needS * ps + needC * pc;

    return cost <= r;
}

int main() {
    cout << "Enter recipe (e.g. BSCBS): ";
    cin >> recipe;

    cout << "Enter available Bread, Sausage, Cheese: ";
    cin >> nb >> ns >> nc;

    cout << "Enter price of Bread, Sausage, Cheese: ";
    cin >> pb >> ps >> pc;

    cout << "Enter available money: ";
    cin >> r;

    // Count ingredients needed for one burger
    for (char ch : recipe) {
        if (ch == 'B')
            cntB++;
        else if (ch == 'S')
            cntS++;
        else if (ch == 'C')
            cntC++;
    }

    long long low = 0;
    long long high = 1e13;
    long long ans = 0;

    while (low <= high) {
        long long mid = low + (high - low) / 2;

        if (canMake(mid)) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    cout << "\nMaximum burgers that can be made = " << ans << endl;

    return 0;
}