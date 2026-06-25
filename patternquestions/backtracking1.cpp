#include <iostream>
using namespace std;

void generateSubsequences(string str, int index, string current) {
    if (index == str.length()) {
        cout << "\"" << current << "\"" << endl;
        return;
    }

    // Include current character
    generateSubsequences(str, index + 1, current + str[index]);

    // Exclude current character
    generateSubsequences(str, index + 1, current);
}

int main() {
    string str = "abc";
    generateSubsequences(str, 0, "");
    return 0;
}