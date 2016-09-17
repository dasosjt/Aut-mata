#ifndef TREE_H
#define TREE_H

#include <string>
#include <stack>
#include <vector>


using namespace std;

struct node{
  char key_value;
  node *left;
  node *right;
  node *parent;
  int id_number;
  bool voidable;
  vector<int> firstpos;
  vector<int> lastpos;
  vector<int> nextpos;
};

class Tree{
  public:
    tree();
    void parse(string exprsn);
    void parseToTree();
    void printLevel(node* root, int level);
    void display();
    int priority(char a);
    node* getRoot();
    vector<char > getL();

  private:
    stack<char> expressionRPN;
    stack<char> operations;
    stack<char> expression;
    stack<node*> nodes;
    node* root;
    vector<char> L;

};

#endif
