#include "tree.h"
#include <iostream>
#include <string>
#include <iomanip>


using namespace std;

Tree::tree(){
  root = NULL;
}

void Tree::printLevel(node* root, int indent){
  if (root != NULL){
       printLevel(root->left, indent + 5);
       if (indent > 0)
           cout << std::setw(indent) << " ";
       cout << root->key_value << endl;
       printLevel(root->right, indent + 5);
   };
}

void Tree::display(){
  printLevel(root, 0);
}

node* Tree::addNode(string key){
  node* n = new node;
  n->key_value = key;
  n->right = NULL;
  n->left = NULL;
  cout << "Nuevo nodo con valor "+ n->key_value << endl;
  return n;
}

void Tree::addRightNode(node* current){
  cout << "Creando nodo derecho " << expression.front() << endl;
  current->right = new node;
  current->right->key_value = expression.front();
  expression.pop();
  current->right->right = NULL;
  current->right->left = NULL;
  current->right->parent = current;
}

void Tree::addLeftNode(node* current){
  cout << "Creando nodo izquierdo " << expression.front() << endl;
  current->left = new node;
  current->left->key_value = expression.front();
  expression.pop();
  current->left->right = NULL;
  current->left->left = NULL;
  current->left->parent = current;
}


void Tree::parse(string exprsn){
  if(exprsn.size() > 0){
    string first = exprsn.substr(0,1);
    string last = exprsn.substr(1,exprsn.size());
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
    root = new node;
    string str_expression = "";
    while(expressionRPN.size()>0){
      str_expression.append(expressionRPN.top());
      expression.push(expressionRPN.top());
      expressionRPN.pop();
    }
    cout << str_expression << endl;
    parseToTree(root);
  };
}

void Tree::parseToTree(node* current){
  if(expression.size() > 0){
    cout << expression.size() << endl;
    if(current->key_value.size() != 0){
      cout << "current->key_value NOT empty" << endl;
      if(current->key_value == "|" || current->key_value == "^" || current->key_value == "*"){
        cout << "Current key value "<< current->key_value << endl;
        if(current->right == NULL){
          if(expression.size() > 0){
            addRightNode(current);
          };
          parseToTree(current->right);
        } else if (current->left == NULL){
          if(expression.size() > 0){
            addLeftNode(current);
          };
          parseToTree(current->left);
        } else {
          cout << "Nos vamos con el padre " << endl;
          parseToTree(current->parent);
        }
      } else {
        cout << "Current key value "<< current->key_value << endl;
        if(current->right == NULL && current->parent->left != NULL){
          if(current->parent->parent->left == NULL){
              cout << "Nos vamos con el abuelo " << endl;
              parseToTree(current->parent->parent);
          }
          if(expression.size() > 0){
            addRightNode(current);
          };
          parseToTree(current->right);
        } else if (current->left == NULL && current->parent->left != NULL){
          if(current->parent->parent->left == NULL){
              cout << "Nos vamos con el abuelo " << endl;
              parseToTree(current->parent->parent);
          }
          if(expression.size() > 0){
            addLeftNode(current);
          };
          parseToTree(current->left);
        } else {
         cout << "Nos vamos con el padre " << endl;
         parseToTree(current->parent);
       };
      }
    } else {
      cout << "Creando ROOT " << expression.front() << endl;
      current->key_value = expression.front();
      expression.pop();
      current->right = NULL;
      current->left = NULL;
      current->parent = NULL;
      parseToTree(current);
    }
  };
}
