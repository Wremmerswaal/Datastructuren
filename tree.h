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



// Class that stores the expression tree.
class Tree{
    Node* root;
    bool bracketed;     // stores if the current expression part
                        // is already bracketed, see 'print_tree()'.
    bool tree_impossible;

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
        out << "\t" + to_string(count) + " [label=\"" + n->left->str_rep + "\"] \n";
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
        out << "\t" + to_string(count) + " [label=\"" + n->right->str_rep + "\"] \n";
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
    else if(n->str_rep=="cos"){
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
    else if(n->str_rep=="sin"){
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
                help->left = new Node(n->right->str_rep);
                help->right = n;
                delete_node(help->right->right);
                help->right->right = new Node("-");
                help->right->right->left = new Node(n->right->str_rep);
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

// TODO 
    // cout << "IN PRINT" << endl;
    // n -> print_node();
    // cout << "\n END" << endl;

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
    // cout << "TESTING" << endl;
    // n -> left -> print_node();
    // cout << "\nEND" << endl;

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

    if (n -> left -> is_zero()){
        // cout << "LEFT IS ZERO\n";
        // n -> left -> print_node();
        // cout << endl;
        // n -> right -> print_node();
        // cout << "\nend" << endl;

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

    else if (n -> left -> is_one()){
        if (n -> left -> is_times()){            // result is the right node
            delete_node(n -> left);
            n = n -> right;
        }
        else if (n -> left -> is_power()){        // result is 1
            delete_node(n -> right);
            delete_node(n -> left);
            n -> set_one();
        }
        return n;
    }

    if ( !(n -> right -> is_value() ) ) {
        n -> right = simp(n -> right);
    }

    // TODO
    // cout << "AA" << endl;
    // n -> right -> print_node();
    // cout << '\n' << n -> right -> is_zero() << endl;
    // cout << "\nEND AA" << endl;

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
        // if operator is division, nothing is calculated
    }

    else if (n -> right -> is_one()){
        if ( !(n -> is_addmin()) ){
            delete_node(n -> right);
            n = n -> left;
            return n;
        }
    }

    else if (n -> left -> is_number() && n -> right -> is_number()){
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

// Takes an input string and converts to a command,
// return true if the command is 'end'. Otherwise return false.
bool Tree::menu(string in){
    if (in == "exp"){
        read();
        return 0;
    }
    else if (in == "print"){
        // TODO
        // cout << "IN PRINT" << endl;
        // root -> print_node();
        // cout << "\n" << root -> left;
        // cout << "\nEND" << endl;

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
        out << "digraph G {\n\t1 [label=\"" + root->str_rep + "\"] \n";
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
            // cout << "AFTER SIMP" << endl; TODO
            // root -> print_node();
            // cout << "\nEND" << endl;
            return 0;
        }
    }
    
    else if(in == "diff"){
        diff(root);
        cout << endl;
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