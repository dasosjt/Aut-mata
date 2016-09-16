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
  cout << "----- Set ID Number -----" << endl;
  set_id_number(root);
  cout << "----- Set Voidable -----" << endl;
  set_voidable(root);
  cout << "----- Set Firstpos -----" << endl;
  set_firstpos(root);
  cout << "----- Set Lastpos -----" << endl;
  set_lastpos(root);
  cout << "----- Set Nextpos -----" << endl;
  set_nextpos(root);
}

int AFD::get_new_state(){
  state+=1;
  cout <<"new state "<< state << endl;
  return state;
}

void AFD::set_id_number(node* root){
  if(root != NULL){
    if(root->key_value == '|' || root->key_value == '^' || root->key_value == '*'){
        root->id_number = 0;
    } else {
      root->id_number = get_new_state();
      cout << root->key_value << " with id number " << root->id_number << endl;
    }
    set_id_number(root->left);
    set_id_number(root->right);
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
    if (root->id_number == 0){
      cout << root->key_value << " is voidable " << root->voidable << endl;
    } else {
      cout << root->key_value << " with id number " << root->id_number << " is voidable " << root->voidable << endl;
    }

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
    if (root->id_number == 0){
      cout << root->key_value << " with firstpos { ";
    } else {
      cout << root->key_value << " with id number " << root->id_number << " with firstpos { ";
    }
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
    if (root->id_number == 0){
      cout << root->key_value << " with lastpos { ";
    } else {
      cout << root->key_value << " with id number " << root->id_number << " with lastpos { ";
    }
    copy(root->lastpos.begin(), root->lastpos.end(), ostream_iterator<int>(cout, " "));
    cout << "}"<< endl;
    return root->lastpos;
  }
}

void AFD::set_nextpos(node* root){
  if(root != NULL){
    if(root->key_value == '^'){
      cout << "NODE ^" << endl;
      for(unsigned int i = 0; i<root->left->lastpos.size(); i++){
        node* temp = search_node(root->left, root->left->lastpos[i]);
        temp->nextpos.insert(temp->nextpos.end(), root->right->firstpos.begin(), root->right->firstpos.end());
        sort(temp->nextpos.begin(), temp->nextpos.end());
        cout << "Node with key value "<< temp->key_value << " with id number " << temp->id_number << " with nextpos { ";
        copy(root->right->firstpos.begin(), root->right->firstpos.end(), ostream_iterator<int>(cout, " "));
        cout << "}"<< endl;
      }
    } else if (root->key_value == '*'){
      cout << "NODE *" << endl;
      for(unsigned int i = 0; i<root->lastpos.size(); i++){
        node* temp = search_node(root, root->lastpos[i]);
        temp->nextpos.insert(temp->nextpos.end(), root->firstpos.begin(), root->firstpos.end());
        sort(temp->nextpos.begin(), temp->nextpos.end());
        cout << "Node with key value "<< temp->key_value << " with id number " << temp->id_number << " with nextpos { ";
        copy(temp->nextpos.begin(), temp->nextpos.end(), ostream_iterator<int>(cout, " "));
        cout << "}"<< endl;
      }
    }
    set_nextpos(root->left);
    set_nextpos(root->right);
  }
}

node* AFD::search_node(node* root, int id_number){
  if(root != NULL){
    cout << "Node key value '"<< root->key_value << "' Node id number " << root->id_number << endl;
    cout << " ... searching for " << id_number << endl;
    if(root->key_value == '|' || root->key_value == '^'){
      if(search_node(root->left, id_number) != NULL){
        return search_node(root->left, id_number);
      } else if (search_node(root->right, id_number) != NULL){
        return search_node(root->right, id_number);
      }
    } else if (root->key_value == '*'){
      if(search_node(root->right, id_number) != NULL){
        return search_node(root->right, id_number);
      }
    } else {
      if(root->id_number == id_number){
        return root;
      } else {
        return NULL;
      }
    }
  }
}
