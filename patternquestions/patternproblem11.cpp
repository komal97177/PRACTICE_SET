#include <iostream>
using namespace std;
int main(){
    int n, row,column;
    cout<<"Enter the value of n: ";
    cin>>n;
    char ch='A';
    for(row=1;row<=n;row++){
        for(column=1;column<=row;column++){             
            cout<<ch<<" " ;    }
        cout<<endl;
        ch++;
    }
    return 0;
}
/*
OUTPUT:
Enter the value of n: 5
A 
B B
C C C
D D D D
E E E E E

*/