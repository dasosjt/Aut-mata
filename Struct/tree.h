#ifndef TREE_H
#define TREE_H

#include <string>
#include <stack>
#include <queue>

using namespace std;

struct node{
  string key_value;
  node *left;
  node *right;
};

class Tree{
  public:
    tree();
    node* addNode(string key);
    void parse(string exprsn);
    void addOrNode();
    void addMulNode();
    void addBraNode();
    void addCleanNode();
    void printLevel(node* root, int level);
    void display();

  private:
    queue<string> expressionRPN;
    stack<node*> expression;
    stack<string> brackets;
    stack<string> operations;

};

#endif
