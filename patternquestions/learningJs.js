console.log("HELLO WORLD");
// comment single line 
/* ddd
ddd
dd*/
//arithmetic operations:
 a= 22 ;
 b= 36;
 console.log("a is ",a , "& b is", b);
 console.log("a+b is",a+b);
 console.log("a/b is ", a/b);
 console.log("a*b is", a*b);
 // unary operators 
 console.log("a++ is ", a++);
 console.log("++b is", ++b);
 console.log("++a",++a);
 console.log("b++",b++);
 console.log("Final a & b is",a,b);
 //assignment operators 
console.log("a for add",a+=66);
console.log("b for sub",b-= 2);
console.log("a for division ",a/= 2);
console.log("b for mult",b*=10);
console.log("a&b double**",a**=2,b**=2);
// comparison operators 
console.log("is A == B", a== b);
console.log("is A is not equal ", a!=b);
// if statement 
let age = 13 ;
let mode = "dark";
let color;
// if - else statement 
if (age >= 18 ){
console.log("you can vote ");
}
else {
    console.log("you can,t vote ");
}
 if (mode === "dark"){
    color = "black";
 }
  console.log("the color of background is :", color);
  // alert 
  alert("HELLO");
  let umar = 32 ;
   if(umar <= 19){
    console.log( "you are teenage");
   } else if (umar >= 20){
    console.log("you are an Adult");
   }else{
    console.log("MAR JA SALLA ");

   }
   // for loop 
   for(let  i =1;i<=5;i++){
    console.log("HELLO AARYA");
   }
   // while loop 
   let x = 1 ;
   while(  x<5){
    console.log("i=",x);
    x++ ;
   }
   // do while loop 

// Use a different variable name for the next loop
let w = 1;
do {
  console.log("w =", w);
  w++;
} while(w <= 5);
 //
 console.log("for -of");
//for-of loop 
let str = "KOMAL";
let size = 0;
for(let a of str ){
   console.log("a=",a);
   size++ ;
}
console.log("size od string",size);
// arrays
let marks1 = [99,99,88,77,55,66];
console.log(marks1);
 let WORDS = ["KOMAL","BE","CALM","WHY ","ARE","YC"];
 for(let i = 0 ; i<6 ; i++){
   console.log(WORDS[i]);
 }
// practice question 
let marks = [85,97,44,37,76,60];
let avg ;
let sum = 0 ;
for (let i = 0 ; i < 6; i++){
   sum = sum + marks[i];
}
avg = sum/6 ;
console.log("The avg is ", avg);
// practice problem 2
let price = [250,645,300,900,50];
console.log("THE PRICE ARRAY BEFORE OFFER:",price);
let offer ;
for (let i =0 ; i<5;i++){
   offer = price[i]/10 ;
   price[i]=price[i]-offer ;
}
console.log("THE PRICE ARRAY AFTER OFFER:",price);
/* array 
push();
pop();
toString();
*/
let cars = ["Toyota","Honda"," Kia", "BMW", "Maruti", "Renault", "Land Rover", "Lexus", "MG", "Tata", "Volvo", "Nissan",  "Audi" ] ;
console.log("The actual array is ", cars);
let newcars = ["Porsche", "Jaguar", "Fiat", "Mini", "Maserati", "Ferrari", "Lamborghini", "Bugatti", "Rolls-Royce"];
console.log("The actual array is ", newcars);
cars.push(newcars);
console.log("After using PUsh:",cars);
cars.pop();
console.log("Ater using pop", cars);
newcars.pop();
console.log("After using pop ", newcars);
//functions
// to count vowels 

