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
       printLevel(root->left, indent + 5);
       if (indent > 0)
           cout << std::setw(indent) << " ";
       cout << root->key_value << endl;
       printLevel(root->right, indent + 5);
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
  cout << "Agregando nodo |" <<endl;
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
  if(op.size()>1 && expression.size() > 0){
    cout << "AddBradNode Recursive" << endl;
    string optodo = op.top();
    op.pop();
    if(optodo == "|"){
      addOrNode();
    } else if (optodo == "^"){
      addMulNode();
    };
    addBraNode();
  }else{
    cout << "Agregando nodo ()" <<endl;
    node* braNode = new node;
    braNode->key_value = "()";
    braNode->left = expression.top();
    expression.pop();
    braNode->right = NULL;
    expression.push(braNode);
    if(op.size() > 0 && expression.size() > 0){
      string optodo = op.top();
      op.pop();
      if(optodo == "|"){
        addOrNode();
      } else if (optodo == "^"){
        addMulNode();
      };
    };
  };
}

void Tree::parse(string exprsn){
  if(exprsn.size()>0){
    cout << "Expresion antes de parsear " << exprsn << endl;
    cout << "Size Expression Node " << expression.size() << endl;
    cout << "Size Brackets " << brackets.size() << endl;
    cout << "Size Operacion " << op.size()<< endl;
    string first = exprsn.substr(0,1);
    string last = exprsn.substr(1,exprsn.size());
    if(first == "("){
      if(op.size() == 0 && expression.size() > 0){
        cout << "Operacion ^, sin aplicar" <<endl;
        op.push("^");
      };
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
      cout << "Size Expression Node " << expression.size() << endl;
      cout << "Size Brackets " << brackets.size() << endl;
      cout << "Size Operacion " << op.size()<< endl;
      parse(last);
    }
    if(first == "|"){
      cout << "Operacion |, sin aplicar" <<endl;
      op.push(first);
      parse(last);
    };
    if(first == "b" || first == "a"){
      cout << "Caracter " << first << endl;
      expression.push(addNode(first));
      if(op.size() == 0 && expression.size() > 1){
        cout << "Operacion ^, sin aplicar" <<endl;
        op.push("^");
        parse(last);
      };
      if(op.size() > 0 && expression.size() > 0 && brackets.size() == 0){
        string optodo = op.top();
        op.pop();
        if(optodo == "|"){
          addOrNode();
        } else if (optodo == "^"){
          addMulNode();
        }
      };
      parse(last);
    };
  };
}
