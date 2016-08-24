#include "tree.h"
#include <iostream>
#include <string>
#include <iomanip>


using namespace std;

Tree::tree(){

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
  printLevel(expression.top(), 0);
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
    if(operations.size() > 1){
      string todo = operations.top();
      operations.pop();
      if(todo == "|"){
        addOrNode();
      } else if (todo == "^"){
        addMulNode();
      };
    }
    cout << "Agregando nodo ()" <<endl;
    node* braNode = new node;
    braNode->key_value = "()";
    braNode->left = expression.top();
    expression.pop();
    braNode->right = NULL;
    expression.push(braNode);
}

void Tree::addCleanNode(){
    cout << "Agregando nodo *" <<endl;
    node* cleanNode = new node;
    cleanNode->key_value = "*";
    cleanNode->left = expression.top();
    expression.pop();
    cleanNode->right = NULL;
    expression.push(cleanNode);
}

void Tree::parse(string exprsn){
  cout << "size Queue RPN " << expressionRPN.size() << endl
       << "size Stack Op "  << operations.size() << endl;
  if(exprsn.size() > 0){
    string first = exprsn.substr(0,1);
    string last = exprsn.substr(1,exprsn.size());
    cout << first << endl;
    if(first == ")"){
      while(operations.top() != "("){
        expressionRPN.push(operations.top());
        operations.pop();
      };
      operations.pop();
    };
    if(first == "|" || first == "*" || first == "(" || first == "^"){
      if(operations.size() > 0){
        if(first == "|" && (operations.top() == "|" ||  operations.top() == "^" || operations.top() == "*")){
          while (operations.size() != 0){
            expressionRPN.push(operations.top());
            operations.pop();
          };
        };
        if(first == "^" && (operations.top() == "^" || operations.top() == "*")){
          while (operations.size() != 0){
            expressionRPN.push(operations.top());
            operations.pop();
          };
        };
      };
      operations.push(first);
    }else if (first != ")"){
      expressionRPN.push(first);
    };
    parse(last);
  } else {
    while(operations.size()>0){
      expressionRPN.push(operations.top());
      operations.pop();
    };
    while(expressionRPN.size()>0){
      cout << expressionRPN.front() << endl;
      expressionRPN.pop();
    }
  };
}
