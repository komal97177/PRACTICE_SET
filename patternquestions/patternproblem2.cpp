#include <iostream>
using namespace std;
int main(){
    int n, row,column;
    cout<<"Enter the value of n: ";
    cin>>n;
    for(row=1;row<=n;row++){
        for(column=1;column<=n;column++){
            cout<<"* ";
        }
        cout<<endl;
    }
}
/*
OUTPUT:
Enter the value of n: 4

* * * * 
* * * *
* * * *
* * * *

*/