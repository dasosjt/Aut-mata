
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
       if(root->key_value == char(167)){
         cout << '^' << endl;
       } else if(root->key_value == char(179)){
         cout << '|' << endl;
       } else if(root->key_value == char(241)){
         cout << '*' << endl;
       } else if(root->key_value == char(238)){
         cout << "epsilon" << endl;
       } else {
        cout << root->key_value << endl;
       }
       printLevel(root->left, indent + 6);
   };
}

void Tree::display(){
  printLevel(root, 0);
}

int Tree::priority(char a){
  int temp;
  /*Mapa de prioridades*/
  if(a == char(241)){
    temp = 1;
  } else if (a == char(167)){
    temp = 2;
  } else if (a == char(179)){
    temp = 3;
  }
  return temp;
}

void Tree::parse(string exprsn){
  // delete whitespace
  exprsn.erase(remove(exprsn.begin(), exprsn.end(), ' '), exprsn.end());
  cout << exprsn << endl;
  // .. as range
  for (unsigned int i = 0; i<exprsn.size(); i++){
    if(exprsn.at(i) == char(176)){
      if(exprsn.at(i+1) == char(176)){
        int from = (int) exprsn.at(i-1);
        int to = (int) exprsn.at(i+2);
        //cout << exprsn.at(i-1) << endl;
        //cout << exprsn.at(i+2) << endl;
        exprsn.erase(exprsn.begin()+i+1);
        exprsn.erase(exprsn.begin()+i);
        for (unsigned int j = 1; j< to - from; j++){
            int current = to-j;
            exprsn.insert(i, 1, char(179));
            exprsn.insert(i, 1, current);
            //cout << exprsn << endl;
        }
        exprsn.insert(i, 1, char(179));
      }
    }
  }
  //braces as ?
  cout << "After Range "<< exprsn << endl;
  for (unsigned int i = 0; i<exprsn.size(); i++){
    if(exprsn.at(i) == char(191)){
      exprsn.insert(i+1, 1, char(244));
      exprsn.erase(exprsn.begin()+i);
    } else if(exprsn.at(i) == char(192)){
      exprsn.insert(i+1, 1, char(179));
      exprsn.insert(i+2, 1, char(238));
      exprsn.insert(i+3, 1, char(245));
      exprsn.erase(exprsn.begin()+i);
    }
  }
  //brackets as a kleene
  cout << "After braces as ? " << exprsn << endl;
  for (unsigned int i = 0; i<exprsn.size(); i++){
    if(exprsn.at(i) == char(212)){
      exprsn.insert(i+1, 1, char(244));
      exprsn.erase(exprsn.begin()+i);
    } else if(exprsn.at(i) == char(213)){
      exprsn.insert(i+1, 1, char(245));
      exprsn.insert(i+2, 1, char(241));
      exprsn.erase(exprsn.begin()+i);
    }
  }
  //add concatenation
  cout << "After brackets as kleene " << exprsn << endl;
  for(unsigned int i = 0; i<exprsn.size(); i++){
    if(isalpha(exprsn.at(i)) || exprsn.at(i) == char(245) || exprsn.at(i) == char(241) || exprsn.at(i) == '(' || exprsn.at(i) == ')' || exprsn.at(i) == '.' || exprsn.at(i) == '+' || exprsn.at(i) == '-' || exprsn.at(i) == char(34) || exprsn.at(i) == '=' || exprsn.at(i) == char(39)){
      if(i+1<exprsn.size()){
        if(isalpha(exprsn.at(i+1)) || exprsn.at(i+1) == char(244) || exprsn.at(i+1) == '(' || exprsn.at(i+1) == ')' || exprsn.at(i+1) == '.' || exprsn.at(i+1) == '+' || exprsn.at(i+1) == '-' || exprsn.at(i+1) == char(34) || exprsn.at(i+1) == '=' || exprsn.at(i) == char(39)){
          exprsn.insert(i+1, 1, char(167));
        }
      }
    }
  }
  //print the result after the first parse
  cout << "After concatenation " << exprsn << endl;
  for(int i = 0; i<exprsn.size(); i++){
    //cout << "stack RPN size " << expressionRPN.size() << endl;
    //cout << "stack operations size " << operations.size() << endl;
    if(exprsn.at(i) == char(179) || exprsn.at(i) == char(167) || exprsn.at(i) == char(241)){
      //cout << "Caracter es " << exprsn[i] <<endl;
      while (!operations.empty() && (priority(operations.top()) <= priority(exprsn[i])) && operations.top() != char(244)){
        //cout << "Sacando del stack " << operations.top() <<endl;
        expressionRPN.push(operations.top());
        operations.pop();
      };
      operations.push(exprsn.at(i));
    } else if (exprsn.at(i) == char(244)){
      //cout << "Caracter es " << exprsn[i] <<endl;
      operations.push(exprsn.at(i));
    } else if(exprsn.at(i) == char(245)){
      //cout << "Caracter es " << exprsn[i] <<endl;
      while (operations.top() != char(244)){
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
    if(expression.top() == char(241)){
      node* nodeop = new node;
      nodeop->right = nodes.top();
      nodes.pop();
      nodeop->left = NULL;
      nodeop->key_value = expression.top();
      ////cout <<"Nodo creado "<< * << endl;
      expression.pop();
      nodes.push(nodeop);
    } else if(expression.top() == char(179) || expression.top() == char(167)){
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
