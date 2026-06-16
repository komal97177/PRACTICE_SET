#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int minWindowLength(string s, string t)
{
    vector<int> need(256, 0);
    vector<int> window(256, 0);

    for(char ch : t)
        need[ch]++;

    int required = t.size();
    int formed = 0;

    int left = 0;
    int minLen = INT_MAX;

    for(int right = 0; right < s.size(); right++)
    {
        char ch = s[right];
        window[ch]++;

        if(need[ch] > 0 && window[ch] <= need[ch])
            formed++;

        while(formed == required)
        {
            minLen = min(minLen, right - left + 1);

            char leftChar = s[left];
            window[leftChar]--;

            if(need[leftChar] > 0 &&
               window[leftChar] < need[leftChar])
            {
                formed--;
            }

            left++;
        }
    }

    return (minLen == INT_MAX) ? 0 : minLen;
}

int main()
{
    string S = "adobecodebanc";
    string T = "abcc";

    cout << minWindowLength(S, T);

    return 0;
}