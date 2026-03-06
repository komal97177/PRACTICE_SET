#include <iostream>
using namespace std;
int main(){
    int n, row,column;
    cout<<"Enter the value of n: ";
    cin>>n;
    int count=1;
    for(row=1;row<=n;row++){
        for(column=1;column<=row;column++){
            cout<<count<<" ";
            count++;
        }
        cout<<endl;
    }
    return 0;
}
/*
OUTPUT:
Enter the value of n: 5
1 
2 3
4 5 6
7 8 9 10
11 12 13 14 15

*/