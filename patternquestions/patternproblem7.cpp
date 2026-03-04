#include <iostream>
using namespace std;
int main(){
    int n, row,column;
    cout<<"Enter the value of n: ";
    cin>>n;
    for(row=1;row<=n;row++){
        for(column=1;column<=row;column++){
            cout<<row;
        }
        cout<<endl;
    }
    return 0;
}
/*
OUTPUT:
Enter the value of n: 5
1
22
333
4444
55555
*/