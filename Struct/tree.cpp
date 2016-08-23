#include "tree.h"
#include <iostream>
#include <string>


using namespace std;

Tree::tree(){
    root = NULL;
}

void Tree::insert(string key){
  if(root!=NULL){
    /*add(key, root);*/
  }else{
    root = new node;
    root->key_value = key;
    cout << root->key_value << endl;
    root->left = NULL;
    root->right = NULL;
  }
}
