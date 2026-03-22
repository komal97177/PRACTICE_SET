#include <iostream>
using namespace std;
int main(){
    int n, row,column;
    cout<<"Enter the value of n: ";
    cin>>n;
    for(row=1;row<=n;row++){
        for(column=row;column>=1;column--){
            cout<<column<<" " ;    }
        cout<<endl;
    }
    return 0;
}
/*
OUTPUT:
1 
2 1
3 2 1
4 3 2 1
5 4 3 2 1
*/