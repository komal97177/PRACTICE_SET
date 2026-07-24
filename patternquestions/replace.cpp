#include <iostream>
using namespace std;

void replacePi(char input[], int i) {
    // base case
    // if we reach the end of the string, return
    if (input[i] == '\0') {
        return;
    }

    // if "pi" found
    if (input[i] == 'p' && input[i + 1] == 'i') {

        // shift string right to make space for "3.14"
        // i is the index of 'p', so we need to find the end of the string
        // j will point to the null terminator of the string
        // k will be used to shift characters to the right
        int j = i + 2;
        while (input[j] != '\0') {
            j++;
        }

        // shift characters right by 2 extra positions
        for (int k = j; k >= i + 2; k--) {
            input[k + 2] = input[k];
        }

        // replace "pi" with "3.14"
        input[i] = '3';
        input[i + 1] = '.';
        input[i + 2] = '1';
        input[i + 3] = '4';

        // recursive call after replacement
        replacePi(input, i + 4);
    }
    else {
        // move to next character
        replacePi(input, i + 1);
    }
}

int main() {
    char str[100] = "pippi";

    replacePi(str, 0);

    cout << str << endl;   // 3.14p3.14

    return 0;
}