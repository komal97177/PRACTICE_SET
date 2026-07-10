#include <bits/stdc++.h>
using namespace std;

int minSteps(int n) {
    // Base case: if n is 1, no steps are needed
    if (n == 1)
        return 0;
// Recursive case: calculate the minimum steps for n-1, n/2, and n/3
    int ans = minSteps(n - 1);

    if (n % 2 == 0)
        ans = min(ans, minSteps(n / 2));

    if (n % 3 == 0)
        ans = min(ans, minSteps(n / 3));
  // Return the minimum steps plus one for the current step
    return 1 + ans;
}

int main() {
    int n = 10;
    cout << minSteps(n);
}