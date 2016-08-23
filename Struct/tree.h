#ifndef TREE_H
#define TREE_H

#include <string>

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

  private:
    node *root;

};

#endif
