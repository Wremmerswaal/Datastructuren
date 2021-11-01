#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
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
    Node* remember = new Node;
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


// Takes an input string and converts to a command,
// return true if the command is 'end'. Otherwise return false.
bool Tree::menu(string in){
    if (in == "exp"){
        read();
        return 0;
    }
    else if (in == "print"){
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

    else if(in == "end"){
        delete_tree();
        return 1;
    }
    else {
        return 0;
    }
}

#endif