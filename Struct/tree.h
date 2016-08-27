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
  node *parent;
};

class Tree{
  public:
    tree();
    node* addNode(string key);
    void addLeftNode(node* current);
    void parse(string exprsn);
    void parseToTree(node* current);
    void printLevel(node* root, int level);
    void display();
    void addRightNode(node* current);

  private:
    stack<string> expressionRPN;
    queue<string> expression;
    stack<string> brackets;
    stack<string> operations;
    node* root;

};

#endif
