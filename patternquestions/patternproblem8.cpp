#include <iostream>
using namespace std;
int main(){
    int n, row,column;
    cout<<"Enter the value of n: ";
    cin>>n;
    for(row=1;row<=n;row++){
        for(column=1;column<=row;column++){
            cout<<column;
        }
        cout<<endl;
    }
    return 0;
}
/*
OUTPUT:
Enter the value of n: 4
1
12
123
1234
*/