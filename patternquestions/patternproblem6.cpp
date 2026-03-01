#include <iostream>
using namespace std;
int main(){
    int n,row,column;
    cout<<"Enter the value of n: ";
    cin>>n;
    int count =1;
    for(row=1;row<=n;row++){
        for(column=n;column>=1;column--){
            cout<<count<<" ";
            count++;
        }
        cout<<endl;
    }
}
/*
OUTPUT:
Enter the value of n: 3
1 2 3 
4 5 6
7 8 9

*/