// -------------------------------------------------
// Datastructures assignment 2
// Authors: Liya Charlaganova, Wouter Remmerswaal
// -------------------------------------------------
//
// Implementation of tree.h

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include "tree.h"
#include "node.h"
#include "basic.h"

using namespace std;

Tree::Tree(){
    root = nullptr;
    bracketed = true;
    tree_impossible = false;
}

Tree::~Tree(){
    delete_tree();
    root = nullptr;
}

// Checks and returns whether the tree is empty.
bool Tree::is_empty(){
    return root == nullptr;
}

// Deletes the tree starting at node 'n' as long as the node is
// not a nullptr.
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


// Recursively delete the whole tree.
void Tree::delete_tree(){
    if (root != nullptr){
        delete_node(root);
        root = nullptr;
    }
}

// Reads in from a stringstream and makes the tree (recursively).
// 'is' is the line containing the expression itself with spaces separating
// the oparators and values. Returns the root of the (sub)tree.
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

// Write the expression in DOT expression to 'filename'. Requires the 
// tree to not be empty. Node 'n' is the root of the (sub)tree.
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

// Differentiates the (sub)tree starting from node n.
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

// Print the tree in a recursive fashion using brackets in a recursive fashion.
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

// Reads the expression and creates a tree if the expression is valid. If
// invalid expression, returns false and empties the tree.
bool Tree::read(){
    string line;
    string str;
    tree_impossible = false;

    getline(cin, line);     // get expression
    
    istringstream is(line); // create stringstream for easy iteration over
                            // elements of expression

    delete_tree();

    root = make_tree(is);

    if (tree_impossible || (is >> str)){
        cout << "INVALID INPUT" << endl;
        delete_tree();
        return false;
    }
    return true;
}

// Simplifies the (sub)tree starting from node n. Returns the new (sub)tree
// from that node.
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
            return n;
        }
        else {                                  // result is the right node
            Node* temp = n -> right;
            delete_node(n -> left);
            delete n;
            n = nullptr;
            return temp;
        }
    }

    // Simplification of tree if left leaf is 1.
    else if (n -> left -> is_one()){
        if (n -> left -> is_times()){            // result is the right node
            Node* temp = n -> right;
            delete_node(n -> left);
            delete n;
            n = nullptr;
            return temp;
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
            Node* temp = n -> left;
            delete_node(n -> right);
            delete n;
            n = nullptr;
            return temp;
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
            Node* temp = n -> left;
            delete_node(n -> right);
            delete n;
            return temp;
        }
    }

    // Calculate new node value if both leaves are  numbers
    if (n -> left -> is_number() && n -> right -> is_number()){
        n -> calc_binary();
    }
    else if (n -> same_vars() && (n -> is_min() || n -> is_div()) ){
        delete_node(n -> left); delete_node(n -> right);
        n -> calc_vars();
    }
    return n;
}

// Evaluate the expression starting at node 'n': replace
// all instances of variable "x" in the tree with the double 'x'.
// Node n is required to not be a nullptr.
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
        if (is_empty()){
            cout << "EMPTY TREE" << endl;
            return 0;
        }
        bracketed = true;
        print_tree(root);
        cout << endl;
        return 0;
    }
    else if(in == "dot"){
        if (is_empty()){
            cout << "EMPTY TREE" << endl;
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
        if (is_empty()){
            cout << "EMPTY TREE" << endl;
            return 0;
        }
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
        if (is_empty()){
            cout << "EMPTY TREE" << endl;
            return 0;
        }
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