#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
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
            cout << "'diff' to differentiate the expression" << endl;
            cout << "'end' to exit the program" << endl;
            cout << endl;
            
            cin >> in;
            exit = T.menu(in);
        }
    }
    return 0;
}