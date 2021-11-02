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

    public:
        Tree();
        ~Tree();
        void read();
        Node* make_tree(istringstream &is);
        void delete_node(Node* n);
        void delete_tree();
        void print_dot(Node*n, int &count, string filename);
        bool menu(string in);
        void print_tree(Node* n);
        void diff(Node* n);
        Node* simp(Node* n);
        void eval(Node* n, double x);
};

Tree::Tree(){
    root = nullptr;
    bracketed = true;
    tree_impossible = false;
}

Tree::~Tree(){
    delete_tree();
    root = nullptr;
}

// Deletes the tree starting at node 'n'.
void Tree::delete_node(Node* n){
    if (n -> left != nullptr){
        delete_node(n -> left);
    }
    if (n -> right != nullptr){
        delete_node(n -> right);
    }
    delete n;
    n = nullptr;
}


// Delete entire tree.
void Tree::delete_tree(){
    if (root != nullptr){
        delete_node(root);
        root = nullptr;
    }
}

// Reads in from a stringstream and makes the tree (recursively).
Node* Tree::make_tree(istringstream &is){
    string str;
    if (is >> str){

        Node* n = new Node(str);
        if ( !(n -> is_value()) && !(n -> is_unknown() ) ){
            n -> left = make_tree(is);
        }
        if (n -> is_binary_op()){
            n -> right = make_tree(is);
        }
        return n;
    }
    else{
        tree_impossible = true;
        return nullptr;
    }
}

// Write the expression in DOT expression to 'filename'.
void Tree::print_dot(Node*n, int &count, string filename){
    ofstream out;
    // Node* remember = new Node;
    Node* remember;
    int rememberCount;
    if(n->left != nullptr){
        out.open(filename, ios_base::app);
        out << "\t" + to_string(count) + " [label=\"" + n->left->get_str() + "\"] \n";
        out << "\t" + to_string(count-1) + "->" + to_string(count) + "\n";
        out.close();
        rememberCount = count;
        remember = n;
        n = n->left;
        count++;
        print_dot(n, count, filename);
        n = remember;
    }
    if(n->is_binary_op()){
                n -> print_node();

        out.open(filename, ios_base::app);
        out << "\t" + to_string(count) + " [label=\"" + n->right->get_str() + "\"] \n";
        out << "\t" + to_string(rememberCount-1) + "->" + to_string(count) + "\n";
        out.close();
        n = n->right;
        count++;
        print_dot(n, count, filename);
    }
    return;
}

// Differentiates the current tree
void Tree::diff(Node* n){
    if(n==nullptr){
        return;
    }
    Node* help;
    if(n->is_value()){
        if(n->is_x()){
            help = new Node("1");
            delete_node(n);
            n = help;
            delete_node(help);
        }
        else{
            help = new Node("0");
            delete_node(n);
            n = help;
            delete_node(help);
        }
    }
    else if(n->get_str()=="cos"){
        help = new Node("*");
        help->left = new Node("*");
        help->left->left = new Node("-1");
        help->right = new Node("sin");
        help->right->left = n->left;
        diff(n->left);
        help->left->right = n->left;
        delete_node(n);
        n = help;
        delete_node(help);
    }
    else if(n->get_str()=="sin"){
        help = new Node("*");
        help->right = new Node("cos");
        help->right->left = n->left;
        diff(n->left);
        help->left = n->left;
        delete_node(n);
        n = help;
        delete_node(help);
    }
    else if(n->is_binary_op()){
        if(n->is_power()){
            if(n->right->is_value()&&!n->right->is_x()){
                help = new Node("*");
                help->left = new Node(n->right->get_str());
                help->right = n;
                delete_node(help->right->right);
                help->right->right = new Node("-");
                help->right->right->left = new Node(n->right->get_str());
                help->right->right->left = new Node("1");
                delete_node(n);
                n = help;
                delete_node(help);
            }
        }
        if(n->is_add()){
            diff(n->left);
            diff(n->right);
        }
        if(n->is_times()){
            help = new Node("+");
            help->left = new Node("*");
            help->right = new Node("*");
            help->left->right = n->right;
            help->right->left = n->left;
            diff(n->left);
            diff(n->right);
            help->left->left = n->left;
            help->right->right = n->right;
            delete_node(n);
            n = help;
            delete_node(help);
        }
        if(n->is_div()){
            help = new Node("/");
            help->right = new Node("*");
            help->right->left = n->right;
            help->right->right = n->right;
            
            help->left = new Node("-");
            help->left->left = new Node("*");
            help->left->right = new Node("*");
            help->left->left->right = n->right;
            help->left->right->left = n->left;
            diff(n->left);
            diff(n->right);
            help->left->left->left = n->left;
            help->left->right->right = n->right;
            delete_node(n);
            n = help;
            delete_node(help);
        }
    }
}

// Print the tree infix with minimal brackets (recursively).
void Tree::print_tree(Node*n){
    if (n == nullptr){
        return;
    }

    if (n -> is_trig() ) {                          // sin and cos are always
                                                    // in brackets
        n -> print_node();
        bracketed = true;
        cout << "(";
        print_tree(n -> left);
        cout << ")";
    }
    else{
        if ( n -> needs_brackets() && !bracketed){  // plus or minus
            cout << "(";
            print_tree(n -> left);
            n -> print_node();
            print_tree(n -> right);
            cout << ")";
        }
        else if (n -> is_div()) {                   // division node
            cout << "(";
            bracketed = true;
            print_tree(n -> left);
            cout << ")";
            n -> print_node();
            cout << "(";
            bracketed = true;
            print_tree(n -> right);
            cout << ")";
        }
        else {                          // binary operator node,
                                        // or the current expression is already
                                        // in brackets
            print_tree(n -> left);
            n -> print_node();
            print_tree(n -> right);
        }
    }
    bracketed = false;
}

// Gets the line for the expression and makes the tree.
void Tree::read(){
    string line;
    string str;
    getline(cin, line);
    
    istringstream is(line);

    delete_tree();

    root = make_tree(is);
    if (tree_impossible || (is >> str)){
        delete_tree();
    }
}

Node* Tree::simp(Node* n){
    if ( !(n -> left -> is_value() ) ) {    // left node is not a leaf,
                                            // find leaf
        n -> left = simp(n -> left);
    }

    if (n -> is_trig()) {               // if the node is a trigonometric function,
                                        // it can immediately be simplified
        if (n -> left -> is_number()){
            n -> calc_trig();
        }
        return n;
    }

    // Simplification of tree if left leaf is 0.
    if (n -> left -> is_zero()){
        if ( ! (n -> is_addmin()) ){    // result is 0
            delete_node(n -> left);
            delete_node(n -> right);
            n -> set_zero();
        }
        else {                                  // result is the right node
            delete_node(n -> left);
            n = n -> right;
        }
        return n;
    }

    // Simplification of tree if left leaf is 1.
    else if (n -> left -> is_one()){
        if (n -> left -> is_times()){            // result is the right node
            delete_node(n -> left);
            n = n -> right;
            return n;
        }
        else if (n -> left -> is_power()){        // result is 1
            delete_node(n -> right);
            delete_node(n -> left);
            n -> set_one();
            return n;
        }
    }

    if ( !(n -> right -> is_value() ) ) {
        n -> right = simp(n -> right);
    }

    // Simplification of tree if right leaf is 0.
    if (n -> right -> is_zero()){
        if (n -> is_addmin()){
            delete_node(n -> right);
            n = n -> left;
        }
        else if ( (n -> is_power()) | n -> is_times()){
            delete_node(n -> left);
            delete_node(n -> right);
            if (n -> is_power() ) { n -> set_one(); }
            else { n -> set_zero(); }
        }
        return n;   // if operator is division, nothing is calculated,
                    // division by 0 is kept in expression
    }

    // Simplification of tree if right leaf is 1.
    else if (n -> right -> is_one()){
        if ( !(n -> is_addmin()) ){
            delete_node(n -> right);
            n = n -> left;
            return n;
        }
    }

    if (n -> left -> is_number() && n -> right -> is_number()){
        n -> calc_binary();
    }
    else if (n -> left -> is_var() && n -> right -> is_var()){
        if (n -> same_vars() ){
            delete_node(n -> left); delete_node(n -> right);
            n -> calc_vars();
        }
    }
    return n;
}

// Evaluate the expression starting at node 'n': replace
// all instances of variable "x" in the tree with the double 'x'.
void Tree::eval(Node* n, double x){
    if (n -> is_value()){
        if (n -> is_x()){
            n -> replace_x(x);
            return;
        }
    }
    if (n -> left != nullptr) {eval(n -> left, x);}
    if (n -> right != nullptr) {eval(n -> right, x);}
}

// Takes an input string and converts to a command,
// return true if the command is 'end'. Otherwise return false.
bool Tree::menu(string in){
    string x_str;
    double x;

    if (in == "exp"){
        read();
        return 0;
    }
    else if (in == "print"){
        bracketed = true;
        print_tree(root);
        cout << endl;
        return 0;
    }
    else if(in == "dot"){
        if (root == nullptr){
            return 0;
        }

        int count = 2;
        string filename;
        cin.ignore();
        getline(cin, filename);
        ofstream out(filename);
        out << "digraph G {\n\t1 [label=\"" + root->get_str() + "\"] \n";
        out.close();
        print_dot(root, count, filename);
        out.open(filename, ios_base::app);
        out << "}";
        out.close();
        return 0;
    }
    else if(in == "simp"){
        if ( (root == nullptr) | (root -> is_value()) | 
             (root -> is_unknown() ) ){
            return 0;
        }
        else {
            root = simp(root);
            return 0;
        }
    }
    
    else if(in == "diff"){
        diff(root);
        cout << endl;
        return 0;
    }

    else if(in == "eval"){
        cin >> x_str;
        if (all_digits(x_str)){
            x = stod(x_str);
            eval(root, x);
            simp(root);
        }
        return 0;
    }

    else if(in == "end"){
        delete_tree();
        return 1;
    }
    else {
        return 0;
    }
}

#endif