#ifndef BASIC_H
#define BASIC_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>

using namespace std;

const double pi = 3.14159;
const double MinVal = 0.00001;

// Check if the string only contains letters.
bool all_alpha(string str){
    int i=0;
    while(str[i]){
        if (!isalpha(str[i])){
            return false;
        }
        i++;
    }
    return true;
}

// Check if input is a positive or negative number (double).
bool all_digits(string str){
    bool dot = false;
    int i=0;
    if (str[i] == '-'){
        i++;
    }
    while(str[i]){
        if ( (str[i] == '.') && !dot){
            dot = true;
        }
        else if (!isdigit(str[i])){
            return false;
        }
        i++;
    }
    return true;
}

#endif