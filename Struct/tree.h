#ifndef TREE_H
#define TREE_H

#include <string>
#include <stack>
#include <queue>

using namespace std;

struct node{
  char key_value;
  node *left;
  node *right;
  node *parent;
};

class Tree{
  public:
    tree();
    void parse(string exprsn);
    void parseToTree();
    void printLevel(node* root, int level);
    void display();
    int priority(char a);

  private:
    stack<char> expressionRPN;
    stack<char> operations;
    stack<char> expression;
    stack<node*> nodes;
    node* root;

};

#endif
