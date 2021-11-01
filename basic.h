#ifndef BASIC_H
#define BASIC_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

// Check if the string only contains letters.
bool is_alpha(string str){
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
bool is_number(string str){
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