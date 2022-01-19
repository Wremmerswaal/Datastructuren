// -------------------------------------------------
// Datastructures assignment 2
// Authors: Liya Charlaganova, Wouter Remmerswaal
// -------------------------------------------------
//
// This file contains the class 'Node' which is a single node in the
// expression tree.

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
    private:
        union value var;
        operators oper;
        string str_rep;         // printable representation,
                                // equal to input

    public:
        Node();
        Node(string str);
        string get_str();

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
        bool is_min();
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
};

#endif
