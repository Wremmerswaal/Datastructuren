// -------------------------------------------------
// Datastructures assignment 2
// Authors: Liya Charlaganova, Wouter Remmerswaal
// -------------------------------------------------
//
// Implementation of node.h

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include "node.h"
#include "basic.h"

using namespace std;

Node::Node(){
    left = nullptr;
    right = nullptr;
    x = false;
}

// Constructor which identifies the type of the node
// based on the input 'str'.
Node::Node(string str){
    left = nullptr;
    right = nullptr;
    x = false;

    str_rep = str;
    if (str == "-") {oper = MINUS;}
    else if (str == "+") {oper = PLUS;}
    else if (str == "*") {oper = MULTIPLY;}
    else if (str == "/") {oper = DIVIDE;}
    else if (str == "^") {oper = POWER;}
    else if (str == "sin") {oper = SIN;}
    else if (str == "cos") {oper = COS;}
    else if (str == "pi") {oper = NUM; var.number = pi;}
    else if (all_alpha(str)) {
        oper = VAR; 
        var.variable = str[0];
        if(str == "x"){
            x = true;
        }
    }
    else if (all_digits(str)) {oper = NUM; var.number = stod(str);}
    else {oper = UNKNOWN; str_rep = '?';}       // used for bad input,
                                                // treated as variable
}

string Node::get_str(){
    return str_rep;
}

// Check if the node is the variable "x"
bool Node::is_x(){
    return (x);
}

// Replace the variable "x" with the double 'x'.
void Node::replace_x(double x){
    oper = NUM;
    var.variable = ' ';
    var.number = x;
    str_rep = to_string(x);
    x = false;
}

// Check whether the node is a variable or number.
bool Node::is_value(){
    return ( (oper == VAR) | (oper == NUM) );
}

// Check if the node is a variable.
bool Node::is_var(){
    return (oper == VAR);
}

// Check if the node is a number.
bool Node::is_number(){
    return (oper == NUM) ;
}

// Check if the node is equal to 0.
bool Node::is_zero(){
    return ( (oper == NUM) && (abs(var.number) < MinVal) );
}

// Check if the node is equal to 1
bool Node::is_one(){
    return ( (oper == NUM) && var.number == 1);
}

// Check whether the node is a binary operator (+, -, x, /, ^).
bool Node::is_binary_op(){
    return ( !is_value() &&
             !is_trig() && (oper != UNKNOWN));
}

// Check whether the node is 'unknown'.
bool Node::is_unknown(){
    return (oper == UNKNOWN);
}

// Check whether the node is a trigonometric operator.
bool Node::is_trig(){
    return ( (oper == SIN) | (oper == COS) );
}

// Check whether the node is a division (/) character.
bool Node::is_div(){
    return (oper == DIVIDE);
}

// Check if the node is a multiplication (*) character.
bool Node::is_times(){
    return (oper == MULTIPLY);
}

// Check if the node is a power (^) character.
bool Node::is_power(){
    return (oper == POWER);
}

// Check if the node is plus or minus.
bool Node::is_addmin(){
    return ((oper == PLUS) | (oper == MINUS));
}

// Check if the node is the addition (+) character
bool Node::is_add(){
    return (oper == PLUS);
}

// Check if the node is the minus (-) character
bool Node::is_min(){
    return (oper == MINUS);
}

// Check whether the operator needs brackets (plus or minus)
bool Node::needs_brackets(){
    return ( (oper == MINUS) | (oper == PLUS) );
}

// Prints the string representation of the node
void Node::print_node(){
    cout << str_rep;
}

// Set the node value to 0. Leafs are set to nullptrs.
void Node::set_zero(){
    oper = NUM;
    var.number = 0;
    var.variable = ' ';
    str_rep = to_string(var.number);
    left = nullptr;
    right = nullptr;
}

// Set the node to 1. Leafs are set to nullptrs.
void Node::set_one(){
    oper = NUM;
    var.number = 1;
    var.variable = ' ';
    str_rep = to_string(var.number);
    left = nullptr;
    right = nullptr;
}

// Perform trigonometric operation and remove the leaf.
void Node::calc_trig(){
    if (oper == SIN){
        var.number = sin(left -> var.number);
    }
    else {
        var.number = cos(left -> var.number);
    }
    if (abs(var.number) < MinVal) {
        var.number = 0;
    }
    var.variable = ' ';
    oper = NUM;
    str_rep = to_string(var.number);

    delete left;
    left = nullptr;
}

// Perform binary operation and remove leafs. Requires both leaves
// to be numbers.
void Node::calc_binary(){
    if (oper == PLUS) {var.number = left -> var.number + right -> var.number;}
    else if (oper == MINUS) {var.number = left -> var.number - right -> var.number;}
    else if (oper == MULTIPLY) {var.number = left -> var.number * right -> var.number;}
    else if (oper == POWER) {var.number = pow(left -> var.number, right -> var.number);}
    else if (oper == DIVIDE) {var.number = left -> var.number / right -> var.number;}
    else {return;} // weird operator

    delete left; delete right;
    left = nullptr; right = nullptr;
    oper = NUM;
    str_rep = to_string(var.number);
}

// Check and return whether the two leafs contain the same 
// variable (char, not number).
bool Node::same_vars(){
    return  ( (left -> var.variable == right -> var.variable)
            && (left -> get_str() == right -> get_str()) );
}

// Perform calculation on variables. Requires operator to be minus or
// division
void Node::calc_vars(){
    if (oper == MINUS){
        set_zero();
    }
    else if (oper == DIVIDE){
        set_one();
    }
}