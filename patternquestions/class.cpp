#include <iostream>
using namespace std;

// yeh mena ek class banayi hai jiska naam hai teachers

class Teachers{
    //yeh mene public access specifier use kiya hai jiska matlab hai ki iske andar jo bhi 
    // data members ya functions honge wo sabhi public honge aur unhe hum class ke bahar se bhi access kar sakte hai
    public:
    // yeh mene class ke andar kuch data members banaye hai ya usme kuch properties daali hai
       char name[100];
       int age;
       int salary ;
        };

int main(){
    // yeh mene ek object banaya hai teachers class ka jiska naam hai teacher1
    Teachers teacher1;
    // yeh mene teacher1 object ke data members ko access kiya hai aur unme kuch values daali hai
    cout<<"Enter name of the teacher: "<<endl;
    cin>>teacher1.name;
    cout<<"Enter age of the teacher: "<<endl;
    cin>>teacher1.age;
    cout<<"Enter salary of the teacher: "<<endl;
    cin>>teacher1.salary;

    // yeh mene teacher1 object ke data members ko access kiya hai aur unki values print ki hai
    cout<<"Name of the teacher: "<<teacher1.name<<endl;
    cout<<"Age of the teacher: "<<teacher1.age<<endl;
    cout<<"Salary of the teacher: "<<teacher1.salary<<endl;
    return 0;
}