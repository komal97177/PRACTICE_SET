#include <iostream>
using namespace std;
int main(){
    int n,row,column;
    cout<<"Enter the value of n: ";
    cin>>n;
    for(row=1;row<=n;row++){
        for(column=n;column>=1;column--){
            cout<<column;
        }
        cout<<endl;
    }
}
/*
OUTPUT:
Enter the value of n: 5
54321
54321
54321
54321
54321
*/