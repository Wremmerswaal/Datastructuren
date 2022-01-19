// -------------------------------------------------
// Datastructures assignment 2
// Authors: Liya Charlaganova, Wouter Remmerswaal
// -------------------------------------------------
//
// Main function used to run the program. Run with option 'd' for debug mode
// (menu will not be printed).
// This program will take in an expression and can simplify it, evaluate it,
// differentiate it and save it to a .dot file. Expression must be in prefix
// notation.

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include "node.h"
#include "tree.h"
#include "basic.h"

using namespace std;


int main(int argc, char** argv){
    bool debugMode;
    bool exit = false;
    string in;

    Tree T;

    if(argc < 2){
        debugMode = false;
    }
    else{
        if (std::string(argv[1]) == "d"){
            debugMode = true;
        }
    }

    if (debugMode){
        while(!exit){
            cin >> in;
            exit = T.menu(in);
        }
    }
    else{
        while(!exit){
            cout << endl;
            cout << "Commands: " << endl;
            cout << "'exp' <expression> to read in an expression." << endl;
            cout << "'print' to print the expression in infix notation."
                 << endl;
            cout << "'dot' <file> to write the expression in DOT";
            cout << " notation to 'file'." << endl;
            cout << "'simp' to simplify expression" << endl;
            cout << "'diff' to differentiate the expression" << endl;
            cout << "'eval' <value> to evaluate expression for x=value" << endl;
            cout << "'end' to exit the program" << endl;
            cout << endl;
            
            cin >> in;
            exit = T.menu(in);
        }
    }
    return 0;
}