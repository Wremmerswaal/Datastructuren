// -------------------------------------------------
// Datastructures assignment 2
// Authors: Liya Charlaganova, Wouter Remmerswaal
// -------------------------------------------------
// 
// This file contains basic constants and functions


#ifndef BASIC_H
#define BASIC_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>

using namespace std;

const double pi = 3.14159;
const double MinVal = 0.00001;      // less than this is considered 0

// Check if the string only contains letters.
bool all_alpha(string str);

// Check if input is a positive or negative number (double).
bool all_digits(string str);

#endif