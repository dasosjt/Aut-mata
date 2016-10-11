
#include "tree.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <iterator>


using namespace std;

Tree::Tree(){
  root = NULL;
}

node* Tree::getRoot(){
  return root;
}

void Tree::printLevel(node* root, int indent){
  if (root != NULL){
    /*Imprimo el nivel si no es NULL*/
    /*Le agrego cierta identacion*/
       printLevel(root->right, indent + 6);
       if (indent > 0)
           cout << std::setw(indent) << " ";
       cout << root->key_value << endl;
       printLevel(root->left, indent + 6);
   };
}

void Tree::display(){
  printLevel(root, 0);
}

int Tree::priority(char a){
  int temp;
  /*Mapa de prioridades*/
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
  // delete whitespace
  exprsn.erase(remove(exprsn.begin(), exprsn.end(), ' '), exprsn.end());
  // .. as range
  for (unsigned int i = 0; i<exprsn.size(); i++){
    if(exprsn.at(i) == '.'){
      if(exprsn.at(i+1) == '.'){
        int from = (int) exprsn.at(i-1);
        int to = (int) exprsn.at(i+2);
        exprsn.erase(exprsn.begin()+i+1);
        exprsn.erase(exprsn.begin()+i);
        for (unsigned int j = 1; j< to - from; j++){
            int current = from+j;
            exprsn.insert(i+j-1, 1, current);
            exprsn.insert(i+j, 1, '|');
        }
      }
    }
  }
  // + as |
  for (unsigned int i = 0; i<exprsn.size(); i++){
    if(exprsn.at(i) == '+'){
      exprsn.insert(i+1, 1, '|');
      exprsn.erase(exprsn.begin()+i);
    }
  }
  //braces as ?
  for (unsigned int i = 0; i<exprsn.size(); i++){
    if(exprsn.at(i) == '['){
      exprsn.insert(i+1, 1, '(');
      exprsn.erase(exprsn.begin()+i);
    } else if(exprsn.at(i) == ']'){
      exprsn.insert(i+1, 1, '|');
      exprsn.insert(i+2, 1, 'e');
      exprsn.insert(i+3, 1, ')');
      exprsn.erase(exprsn.begin()+i);
    }
  }
  //brackets as a kleene
  for (unsigned int i = 0; i<exprsn.size(); i++){
    if(exprsn.at(i) == '{'){
      exprsn.insert(i+1, 1, '(');
      exprsn.erase(exprsn.begin()+i);
    } else if(exprsn.at(i) == '}'){
      exprsn.insert(i+1, 1, ')');
      exprsn.insert(i+2, 1, '*');
      exprsn.erase(exprsn.begin()+i);
    }
  }
  //add concatenation
  for(unsigned int i = 0; i<exprsn.size(); i++){
    if(isalpha(exprsn.at(i)) || exprsn.at(i) == ')' || exprsn.at(i) == '*'){
      if(i+1<exprsn.size()){
        if(isalpha(exprsn.at(i+1)) || exprsn.at(i+1) == '(' ){
          exprsn.insert(i+1, 1, '^');
        }
      }
    }
  }
  //print the result after the first parse
  cout << exprsn << endl;
  for(int i = 0; i<exprsn.size(); i++){
    //cout << "stack RPN size " << expressionRPN.size() << endl;
    //cout << "stack operations size " << operations.size() << endl;
    if(exprsn.at(i) == '|' || exprsn.at(i) == '^' || exprsn.at(i) == '*'){
      //cout << "Caracter es " << exprsn[i] <<endl;
      while (!operations.empty() && (priority(operations.top()) <= priority(exprsn[i])) && operations.top() != '('){
        //cout << "Sacando del stack " << operations.top() <<endl;
        expressionRPN.push(operations.top());
        operations.pop();
      };
      operations.push(exprsn.at(i));
    } else if (exprsn.at(i) == '('){
      //cout << "Caracter es " << exprsn[i] <<endl;
      operations.push(exprsn.at(i));
    } else if(exprsn.at(i) == ')'){
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
      expressionRPN.push(exprsn.at(i));
      if(count(L.begin(), L.end(), exprsn[i]) == 0){
        L.push_back(exprsn.at(i));
      }
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

vector<char > Tree::getL(){
  return L;
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
      ////cout <<"Nodo creado "<< * << endl;
      expression.pop();
      nodes.push(nodeop);
    } else if(expression.top() == '|' || expression.top() == '^'){
      node* nodeop = new node;
      nodeop->right = nodes.top();
      nodes.pop();
      nodeop->left = nodes.top();
      nodes.pop();
      nodeop->key_value = expression.top();
      ////cout <<"Nodo creado "<< |  o  ^ << endl;
      expression.pop();
      nodes.push(nodeop);
    }else{
      node* nodex = new node;
      nodex->right = NULL;
      nodex->left = NULL;
      nodex->key_value = expression.top();
      ////cout <<"Nodo creado "<< cierto caracter ingresado << endl;
      expression.pop();
      nodes.push(nodex);
    };
  };
  root = new node;
  root = nodes.top();
}
