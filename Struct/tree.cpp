#include "tree.h"
#include <iostream>
#include <string>
#include <iomanip>


using namespace std;

Tree::tree(){
  root = NULL;
}

node* Tree::getRoot(){
  return root;
}

void Tree::printLevel(node* root, int indent){
  if (root != NULL){
       printLevel(root->right, indent + 5);
       if (indent > 0)
           cout << std::setw(indent) << " ";
       cout << root->key_value << endl;
       printLevel(root->left, indent + 5);
   };
}

void Tree::display(){
  printLevel(root, 0);
}

int Tree::priority(char a){
  int temp;
  if(a == '*'){
    temp = 1;
  } else if (a == '^'){
    temp = 2;
  } else if (a == '|'){
    temp = 3;
  }
  return temp;
}

void Tree::parse(string exprsn){
  for(int i = 0; i<exprsn.length(); i++){
    //cout << "stack RPN size " << expressionRPN.size() << endl;
    //cout << "stack operations size " << operations.size() << endl;
    if(exprsn[i] == '|' || exprsn[i] == '^' || exprsn[i] == '*'){
      //cout << "Caracter es " << exprsn[i] <<endl;
      while (!operations.empty() && (priority(operations.top()) <= priority(exprsn[i])) && operations.top() != '('){
        //cout << "Sacando del stack " << operations.top() <<endl;
        expressionRPN.push(operations.top());
        operations.pop();
      };
      operations.push(exprsn[i]);
    } else if (exprsn[i] == '('){
      //cout << "Caracter es " << exprsn[i] <<endl;
      operations.push(exprsn[i]);
    } else if(exprsn[i] == ')'){
      //cout << "Caracter es " << exprsn[i] <<endl;
      while (operations.top() != '('){
        //cout << "Sacando del stack " << operations.top() <<endl;
        expressionRPN.push(operations.top());
        operations.pop();
        //cout << "POP del stack, el siguiente es " << operations.top() <<endl;
      };
      operations.pop();
    } else {
      //cout << "Caracter es " << exprsn[i] <<endl;
      expressionRPN.push(exprsn[i]);
    };
  };
  //cout << "Sacando lo ultimo del stack" << endl;
  while (!operations.empty()){
    expressionRPN.push(operations.top());
    operations.pop();
  }
  //cout << "Imprimiendo el PRN" << endl;
  while(expressionRPN.size()>0){
    //cout << expressionRPN.top() << endl;
    expression.push(expressionRPN.top());
    expressionRPN.pop();
  }
  //cout << "Parseando al arbol" << endl;
  parseToTree();
}

void Tree::parseToTree(){
  //cout << expression.top() << endl;
  while(expression.size() > 0){
    //cout << expression.size() << endl;
    if(expression.top() == '*'){
      node* nodeop = new node;
      nodeop->right = nodes.top();
      nodes.pop();
      nodeop->left = NULL;
      nodeop->key_value = expression.top();
      ////cout <<"Nodo creado "<< expression.top() << endl;
      expression.pop();
      nodes.push(nodeop);
    } else if(expression.top() == '|' || expression.top() == '^'){
      node* nodeop = new node;
      nodeop->right = nodes.top();
      nodes.pop();
      nodeop->left = nodes.top();
      nodes.pop();
      nodeop->key_value = expression.top();
      ////cout <<"Nodo creado "<< expression.top() << endl;
      expression.pop();
      nodes.push(nodeop);
    }else{
      node* nodex = new node;
      nodex->right = NULL;
      nodex->left = NULL;
      nodex->key_value = expression.top();
      ////cout <<"Nodo creado "<< expression.top() << endl;
      expression.pop();
      nodes.push(nodex);
    };
  };
  root = new node;
  root = nodes.top();
}
