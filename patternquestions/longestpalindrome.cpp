#include <bits/stdc++.h>
using namespace std;

int longestPalindrome(string s) { // O(n) time, O(1) space

// We can use a set to track characters with odd frequency.
//  Each time we see a character, we toggle its presence in the set.
//  If it's already there, we remove it and add 2 to the length (we found a pair).
//  If it's not there, we add it to the set (odd frequency).
//  At the end, if the set is not empty, we can place one character in the center of the palindrome, so we add 1 to the length.

    unordered_set<char> st; // to track characters with odd frequency
    int len = 0;

    for (char c : s) {
        if (st.count(c)) {  // if character already in set, we found a pair
            len += 2;
            st.erase(c);  // remove it from set (even frequency)
        } else {
            st.insert(c);  // if character not in set, add it (odd frequency)
        }
    }

    // if any character left, we can place one in center
    if (!st.empty()) len += 1;

    return len;
}

int main() {
    string s;

    cout << "Enter string: ";
    cin >> s;

    int result = longestPalindrome(s);

    cout << "Longest Palindrome Length: " << result << endl;

    return 0;
}