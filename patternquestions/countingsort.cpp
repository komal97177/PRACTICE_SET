#include <iostream>
#include <map>
using namespace std;

int main()
{
    int n;

    // Take size of array from user
    cout << "Enter number of elements: ";
    cin >> n;

    // Map to store number and its frequency
    map<int, int> frequency;

    cout << "Enter the elements: ";

    // Read elements and count frequency
    for (int i = 0; i < n; i++)
    {
        int num;
        cin >> num;

        frequency[num] = frequency[num] + 1;
    }

    cout << "\nSorted array: ";

    // Print elements according to their frequency
    for (auto pair : frequency)
    {
        int number = pair.first;      // element
        int count = pair.second;      // frequency

        for (int i = 0; i < count; i++)
        {
            cout << number << " ";
        }
    }

    return 0;
}