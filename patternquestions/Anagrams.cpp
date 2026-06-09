#include <iostream>
#include <unordered_map>
using namespace std;

int main() {
    string s1, s2;

    cout << "Enter first string: ";
    cin >> s1;

    cout << "Enter second string: ";
    cin >> s2;

    if (s1.length() != s2.length()) {
        cout << "Strings are NOT Anagrams\n";
        return 0;
    }

    unordered_map<char, int> freq1, freq2;

    // Build frequency maps
    for (char ch : s1)
        freq1[ch]++;

    for (char ch : s2)
        freq2[ch]++;

    // Display freq map of s1
    cout << "\nFrequency Map of s1:\n";
    for (auto &p : freq1) {
        cout << p.first << " -> " << p.second << '\n';
    }

    // Display freq map of s2
    cout << "\nFrequency Map of s2:\n";
    for (auto &p : freq2) {
        cout << p.first << " -> " << p.second << '\n';
    }

    // Compare maps
    if (freq1 == freq2)
        cout << "\nStrings are Anagrams\n";
    else
        cout << "\nStrings are NOT Anagrams\n";

    return 0;
}