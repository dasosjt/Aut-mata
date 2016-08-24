#include "tree.h"
#include <iostream>
#include <string>
#include <iomanip>


using namespace std;

Tree::tree(){
    root = NULL;
}

void Tree::printLevel(node* root, int indent){
  if (root != NULL)
   {
       printLevel(root->left, indent + 4);
       if (indent > 0)
           cout << std::setw(indent) << " ";
       cout << root->key_value << endl;
       printLevel(root->right, indent + 4);
   }
}

void Tree::display(){
  printLevel(expression.top(), 1);
}

node* Tree::addNode(string key){
  node* n = new node;
  n->key_value = key;
  n->right = NULL;
  n->left = NULL;
  cout << "Nuevo nodo con valor "+ n->key_value << endl;
  return n;
}

void Tree::addOrNode(){
  cout << "Agrengado nodo |" <<endl;
  node* orNode = new node;
  orNode->key_value = "|";
  orNode->left = expression.top();
  expression.pop();
  orNode->right = expression.top();
  expression.pop();
  expression.push(orNode);
}

void Tree::addMulNode(){
  cout << "Agregando nodo ^" <<endl;
  node* mulNode = new node;
  mulNode->key_value = "^";
  mulNode->left = expression.top();
  expression.pop();
  mulNode->right = expression.top();
  expression.pop();
  expression.push(mulNode);
}

void Tree::addBraNode(){
  cout << "Agrengando nodo ()" <<endl;
  node* braNode = new node;
  braNode->key_value = "()";
  braNode->left = expression.top();
  expression.pop();
  braNode->right = NULL;
  expression.push(braNode);
}

void Tree::parse(string exprsn){
  if(exprsn.size()>0){
    cout << "Expresion antes de parsear" <<endl;
    cout << exprsn <<endl;
    string first = exprsn.substr(0,1);
    string last = exprsn.substr(1,exprsn.size());
    if(first == "("){
      brackets.push(first);
      cout << "Bracket push" <<endl;
      cout << brackets.size() <<endl;
      parse(last);
    }
    if(first == ")"){
      brackets.pop();
      cout << "Bracket pop" <<endl;
      cout << brackets.size() <<endl;
      addBraNode();
      parse(last);
    }
    if(first == "|"){
      cout << "Operacion |, sin aplicar" <<endl;
      op.push(first);
      parse(last);
    };
    if(first == "b" || first == "a"){
      cout << "Caracter" <<endl;
      cout << first <<endl;
      expression.push(addNode(first));
      if(op.size() > 0 && expression.size() > 1){
        string optodo = op.top();
        op.pop();
        if(optodo == "|"){
          addOrNode();
        };
      } else if (op.size() == 0 && expression.size() > 1){
        addMulNode();
      }
      parse(last);
    };
  };
}

void Tree::generateTree(){
  node* p = expression.top();
  /*cout << p->right->right->left->key_value << endl;*/
}
