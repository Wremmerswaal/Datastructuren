#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
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
        // ~Node();
        bool is_value();
        bool is_binary_op();
        bool is_trig();
        bool is_div();
        bool is_unknown();
        bool needs_brackets();
        void print_node();

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
}

// Node::~Node(){
//     left = nullptr;
//     right = nullptr;
// }

// Constructor which identifies the type of the node
// based on the input 'str'.
Node::Node(string str){
    left = nullptr;
    right = nullptr;

    str_rep = str;
    if (str == "-") {oper = MINUS;}
    else if (str == "+") {oper = PLUS;}
    else if (str == "*") {oper = MULTIPLY;}
    else if (str == "/") {oper = DIVIDE;}
    else if (str == "^") {oper = POWER;}
    else if (str == "sin") {oper = SIN;}
    else if (str == "cos") {oper = COS;}
    else if (is_alpha(str)) {oper = VAR; var.variable = str[0];}
    else if (is_number(str)) {oper = NUM; var.number = stod(str);}
    else {oper = UNKNOWN; str_rep = '?';}       // used for bad input,
                                                // treated as variable
}

// Check whether the node is a variable or number.
bool Node::is_value(){
    return ( (oper == VAR) | (oper == NUM) );
}

// Check whether the node is a binary operator (+, -, x, /, ^).
bool Node::is_binary_op(){
    return ( (oper != NUM) && (oper!=VAR) &&
             !is_trig() && (oper != UNKNOWN));
}

// Check whether the node is 'unknown'.
bool Node::is_unknown(){
    return (oper == UNKNOWN);
}

// Check whether the node is a trigonometric operator.
bool Node::is_trig(){
    return ( (oper == SIN) | (oper==COS) );
}

// Check whether the node is a division character.
bool Node::is_div(){
    return (oper == DIVIDE);
}

// Check whether the operator needs brackets (plus or minus)
bool Node::needs_brackets(){
    return ( (oper == MINUS) | (oper == PLUS) );
}

// Prints the string representation of the node
void Node::print_node(){
    cout << str_rep;
}

#endif
