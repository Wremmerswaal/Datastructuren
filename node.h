#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include "basic.h"

using namespace std;

// Class for a single node in the tree. This class stores the type of the
// node (operators) and the value if it is a variable/number.
class Node{
    enum operators {
        PLUS, MINUS, MULTIPLY, DIVIDE, POWER, SIN, COS, VAR, NUM, UNKNOWN
    };

    union value {
        char variable;
        double number;
    };
    public:
        Node();
        Node(string str);

        bool x;
        bool is_x();
        void replace_x(double x);

        bool is_value();
        bool is_number();
        bool is_var();
        
        bool is_zero();
        bool is_one();
        bool is_binary_op();
        bool is_trig();
        bool is_div();
        bool is_times();
        bool is_power();
        bool is_addmin();
        bool is_add();
        bool is_unknown();
        bool needs_brackets();
        void print_node();

        void set_zero();
        void set_one();
        void calc_trig();
        void calc_binary();
        void calc_vars();
        bool same_vars();

        Node* left;
        Node* right;

        union value var;
        operators oper;
        string str_rep;         // printable representation,
                                // equal to input
};

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

bool Node::is_x(){
    return (x);
}

void Node::replace_x(double x){
    oper = NUM;
    var.variable = ' ';
    var.number = x;
    str_rep = to_string(x);
}

// Check whether the node is a variable or number.
bool Node::is_value(){
    return ( (oper == VAR) | (oper == NUM) );
}

bool Node::is_var(){
    return (oper == VAR);
}

bool Node::is_number(){
    return (oper == NUM) ;
}

bool Node::is_zero(){
    return ( (oper == NUM) && (abs(var.number) < MinVal) );
}

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

// Check whether the node is a division character.
bool Node::is_div(){
    return (oper == DIVIDE);
}

bool Node::is_times(){
    return (oper == MULTIPLY);
}

bool Node::is_power(){
    return (oper == POWER);
}

bool Node::is_addmin(){
    return ((oper == PLUS) | (oper == MINUS));
}

bool Node::is_add(){
    return (oper == PLUS);
}

// Check whether the operator needs brackets (plus or minus)
bool Node::needs_brackets(){
    return ( (oper == MINUS) | (oper == PLUS) );
}

// Prints the string representation of the node
void Node::print_node(){
    cout << str_rep;
}

void Node::set_zero(){
    oper = NUM;
    var.number = 0;
    var.variable = ' ';
    str_rep = to_string(var.number);
    left = nullptr;
    right = nullptr;
}

void Node::set_one(){
    oper = NUM;
    var.number = 1;
    var.variable = ' ';
    str_rep = to_string(var.number);
    left = nullptr;
    right = nullptr;
}

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

bool Node::same_vars(){
    return  (left -> var.variable == right -> var.variable);
}

void Node::calc_vars(){
    if (oper == MINUS){
        set_zero();
    }
    else if (oper == DIVIDE){
        set_one();
    }
}


#endif
