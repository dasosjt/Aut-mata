#ifndef TREE_H
#define TREE_H

#include <string>
#include <stack>

using namespace std;

struct node{
  string key_value;
  node *left;
  node *right;
};

class Tree{
  public:
    tree();
    void insert(string key);
    void parse(string exprsn);
    node* addNode(string key);
    void addOrNode();
    void addMulNode();
    void addBraNode();
    void generateTree();
    void printLevel(node* root, int level);
    void display();

  private:
    node *root;
    stack<node*> expression;
    stack<string> brackets;
    stack<string> op;

};

#endif
