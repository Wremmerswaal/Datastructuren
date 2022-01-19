// -------------------------------------------------
// Datastructures assignment 2
// Authors: Liya Charlaganova, Wouter Remmerswaal
// -------------------------------------------------
//
// This files contains the expression class 'Tree'. This class stores
// the tree and contains all functions to manipulate it.

#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include "node.h"
#include "basic.h"

using namespace std;


// Class that stores the expression tree and its operations.
class Tree{
    Node* root;
    bool bracketed;     // stores if the current expression part
                        // is already bracketed, see 'print_tree()'
    bool tree_impossible;   // true if input expression is not a
                            // possible tree
    Node* make_tree(istringstream &is);
    void delete_node(Node* n);

    public:
        Tree();
        ~Tree();
        bool is_empty();
        bool read();
        void delete_tree();
        void print_dot(Node*n, int &count, string filename);
        bool menu(string in);
        void print_tree(Node* n);
        void diff(Node* n);
        Node* simp(Node* n);
        void eval(Node* n, double x);
};

#endif