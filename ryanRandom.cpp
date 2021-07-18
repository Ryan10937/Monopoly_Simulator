#include"ryanRandom.h"
#include<math.h>
#include<string>
#include<iostream>
#include<ctime>///////
#include"iAmHere.h"

using namespace std;

int ryanRandom(int seed){//returns random number. Static variable should ensure 

    //cout <<"seed is: " << seed << endl;

    int randomNum = 0;
    static int counter = 1;
    counter++;
    

    seed = seed*10;
    seed = seed*seed*seed/3;
    
    if(counter >10){
        counter = 1;
    }
    seed = abs(seed * counter  % 66521);
     
    if (seed >= 9999) {
        string middleDigits = to_string(seed);
        middleDigits = middleDigits.substr((middleDigits.size()/2)-2,4);
        randomNum = stoi(middleDigits);
        return randomNum;
    }
    else {
        return seed;
    }
    
    
    
}