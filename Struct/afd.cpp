#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

int AFD::state;


AFD::AFD(){
  root = NULL;
  state = 0;
}

void AFD::set_root(node* root){
  //cout << root->key_value << endl;
  this->root = root;
  set_id_number(root);
  set_voidable(root);
  set_firstpos(root);
  set_lastpos(root);
}

int AFD::get_new_state(){
  state+=1;
  cout <<"new state "<< state << endl;
  return state;
}

void AFD::set_id_number(node* root){
  if(root != NULL){
    root->id_number = get_new_state();
    cout << root->key_value << " with id number " << state << endl;
    set_id_number(root->right);
    set_id_number(root->left);
  }
}

bool AFD::set_voidable(node* root){
  if(root != NULL){
    char key_value = root->key_value;
    if(key_value == '|'){
      bool temp_right = set_voidable(root->right);
      bool temp_left = set_voidable(root->left);
      if(temp_right || temp_left){
        root->voidable = true;
      } else {
        root->voidable = false;
      }
    } else if(key_value == '^'){
      bool temp_right = set_voidable(root->right);
      bool temp_left = set_voidable(root->left);
      if(temp_right && temp_left){
        root->voidable = true;
      } else {
        root->voidable = false;
      }
    } else if ( key_value == '*'){
      root->voidable = true;
      set_voidable(root->right);
      set_voidable(root->left);
    } else if ( key_value == 'e'){
      root->voidable = true;
    } else {
      root->voidable = false;
    }
    cout << root->key_value << " with id number " << root->id_number << " is voidable " << root->voidable << endl;
    return root->voidable;
  }
}

vector<int> AFD::set_firstpos(node* root){
  if(root != NULL){
    char key_value = root->key_value;
    if (key_value == '|'){
      vector<int> temp_left = set_firstpos(root->left);
      vector<int> temp_right = set_firstpos(root->right);
      root->firstpos = temp_left;
      root->firstpos.insert(root->firstpos.end(), temp_right.begin(), temp_right.end());
    } else if (key_value == '^'){
      vector<int> temp_left = set_firstpos(root->left);
      vector<int> temp_right = set_firstpos(root->right);
      if(root->left->voidable){
        root->firstpos = temp_left;
        root->firstpos.insert(root->firstpos.end(), temp_right.begin(), temp_right.end());
      } else {
        root->firstpos = temp_left;
      }
    } else if (key_value == '*'){
      vector<int> temp_right = set_firstpos(root->right);
      root->firstpos = temp_right;
    } else {
      root->firstpos.push_back(root->id_number);
    }
    cout << root->key_value << " with id number " << root->id_number << " with firstpos { ";
    copy(root->firstpos.begin(), root->firstpos.end(), ostream_iterator<int>(cout, " "));
    cout << "}"<< endl;
    return root->firstpos;
  }
}

vector<int> AFD::set_lastpos(node* root){
  if(root != NULL){
    char key_value = root->key_value;
    if (key_value == '|'){
      vector<int> temp_left = set_lastpos(root->left);
      vector<int> temp_right = set_lastpos(root->right);
      root->lastpos = temp_left;
      root->lastpos.insert(root->lastpos.end(), temp_right.begin(), temp_right.end());
    } else if (key_value == '^'){
      vector<int> temp_left = set_lastpos(root->left);
      vector<int> temp_right = set_lastpos(root->right);
      if(root->right->voidable){
        root->lastpos = temp_right;
        root->lastpos.insert(root->firstpos.end(), temp_left.begin(), temp_left.end());
      } else {
        root->lastpos = temp_right;
      }
    } else if (key_value == '*'){
      vector<int> temp_right = set_lastpos(root->right);
      root->lastpos = temp_right;
    } else {
      root->lastpos.push_back(root->id_number);
    }
    cout << root->key_value << " with id number " << root->id_number << " with lastpos { ";
    copy(root->lastpos.begin(), root->lastpos.end(), ostream_iterator<int>(cout, " "));
    cout << "}"<< endl;
    return root->lastpos;
  }
}
