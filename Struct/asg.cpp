#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include "asg.h"


ASG::ASG(){
  vertex_asg* root = new vertex_asg;
  this->root = root;
}

void ASG::create_ProductionsRoot(vector< string > productions){
  for(string p : productions){
    vertex_asg* newp = new vertex_asg;
    newp->id = p;
    newp->production_root = true;
    cout << "New node ASG with id " << p << endl;
    productions_root.push_back(newp);
  }
}

bool ASG::create_ASG(string production_name, string expression){
  auto lambda = [production_name](const vertex_asg* current) {
    return current->id == production_name;
  };
  vector< vertex_asg* >::iterator production_name_it = find_if(begin(productions_root), end(productions_root), lambda);
  if(production_name_it != end(productions_root)){
    //cout << "FOUND " << (*production_name_it)->id <<  endl;
    (*production_name_it)->vertex_to.push_back(vertex_byExpression(expression));
  };
}

vertex_asg* ASG::vertex_byExpression(string expression){
  cout << "Expression looking for | " << expression << endl;
  string productionterm0;
  string productionterm1;
  unsigned int i = 0;
  bool stop = false;
  int quote_mark = 0;
  while(i<expression.size() && !stop){
    if(expression.at(i) == '|' and pbb_signs.empty()){
      productionterm0 = expression.substr(0, i);
      productionterm1 = expression.substr(i+1, expression.size()-i-1);
      stop = true;
    }
    if(expression.at(i) == '(' || expression.at(i) == '{' || expression.at(i) == '['){
      pbb_signs.push(expression.at(i));
    } else if(expression.at(i) == ')' || expression.at(i) == '}' || expression.at(i) == ']'){
      pbb_signs.pop();
    } else if(expression.at(i) == '\"'){
      quote_mark += 1;
    }
    i++;
  }
  while(!pbb_signs.empty()){
    pbb_signs.pop();
  }
  if(!productionterm1.empty() && !productionterm0.empty()){
    vertex_asg* vertex_result = new vertex_asg;
    vertex_result->id = "|";
    vertex_result->production_root = false;
    cout << "New node ASG with id |" << endl;
    vertex_result->vertex_to.push_back(vertex_byTerm(productionterm0)); //vertice 1
    vertex_result->vertex_to.push_back(vertex_byExpression(productionterm1)); //vertice 2
    return vertex_result;
  }else{
    return vertex_byTerm(expression);
  }
}

vertex_asg* ASG::vertex_byTerm(string expression){
  cout << "Term looking for ^ " << expression << endl;
  string factor0;
  string factor1;
  int quote_mark = 0;
  int first_quote_pos = 0;
  factor0 = expression;
  unsigned int i = 0;
  bool stop = false;
  while(expression.find_first_of(" ", 0) == 0){
    cout << "Found space in first pos" << endl;
    expression.erase(0,1);
  }
  while(i<expression.size() && !stop){
    cout <<  expression.at(i) << " at position " << i <<  endl;
    if((expression.at(i) == '(' || expression.at(i) == '{' || expression.at(i) == '[') && quote_mark%2 == 0){
      if(i>0 && pbb_signs.empty() && quote_mark%2 == 0){
        factor0 = expression.substr(0, i);
        factor1 = expression.substr(i, expression.size()-i);
        stop = true;
      }
      pbb_signs.push(expression.at(i));
    } else if((expression.at(i) == ')' || expression.at(i) == '}' || expression.at(i) == ']') && quote_mark%2 == 0){
      pbb_signs.pop();
      if(i>0 && pbb_signs.empty() && quote_mark%2 == 0){
        factor0 = expression.substr(0, i+1);
        factor1 = expression.substr(i+1, expression.size()-i-1);
        stop = true;
      }
    } else if(expression.at(i) == '\"'){
      quote_mark += 1;
      if(quote_mark == 1){
        first_quote_pos = i;
      }
      if(i>0 && quote_mark == 2 && pbb_signs.empty()){
        if(first_quote_pos == 0){
          cout << " first_quote_pos == 0" << endl;
          factor0 = expression.substr(0, i+1);
          factor1 = expression.substr(i+1, expression.size()-i-1);
          cout << " F0 " << factor0 << endl;
          cout << " F1 " <<factor1 << endl;
        }else{
          cout << " first_quote_pos != 0" << endl;
          factor0 = expression.substr(0, first_quote_pos);
          factor1 = expression.substr(first_quote_pos, expression.size()-first_quote_pos);
          cout << " F0 " << factor0 << endl;
          cout << " F1 " << factor1 << endl;
        }
        stop = true;
      }
    }
    i++;
  }
  cout << pbb_signs.size() << endl;
  while(!pbb_signs.empty()){
    pbb_signs.pop();
  }
  if(!factor0.empty() && !factor1.empty()){
    vertex_asg* vertex_result = new vertex_asg;
    vertex_result->id = "^";
    vertex_result->production_root = false;
    cout << "New node ASG with id ^" << endl;
    vertex_result->vertex_to.push_back(vertex_byFactor(factor0)); //vertice 1
    vertex_result->vertex_to.push_back(vertex_byTerm(factor1)); //vertice 2
    return vertex_result;
  }else{
    return vertex_byFactor(factor0);
  }
}

vertex_asg* ASG::vertex_byFactor(string expression){
  cout << "Factor looking for * ? or Ident " << expression << endl;
  string productionterm0;
  productionterm0 = expression.substr(1, expression.size()-2);
  if(expression.at(0) == '(' && expression.at(1) == '.' && expression.at(expression.size()-2) == '.' && expression.at(expression.size()-1) == ')'){
    cout << "Found Semantic Action :D " << expression << endl;
    vertex_asg* vertex_result = new vertex_asg;
    vertex_result->id = expression;
    vertex_result->production_root = false;
    cout << "New node ASG with id " << expression << endl;
    return vertex_result;
  }
  if(expression.at(0) == '(' && expression.at(expression.size()-1) == ')'){
    //no se si devolverlo empaquetado
    return vertex_byExpression(productionterm0);
  } else if(expression.at(0) == '[' && expression.at(expression.size()-1) == ']'){
    vertex_asg* vertex_result = new vertex_asg;
    vertex_result->id = "[]";
    vertex_result->production_root = false;
    cout << "New node ASG with id []" << endl;
    vertex_result->vertex_to.push_back(vertex_byExpression(productionterm0)); //vertice 1
    return vertex_result;
  } else if(expression.at(0) == '{' && expression.at(expression.size()-1) == '}'){
    vertex_asg* vertex_result = new vertex_asg;
    vertex_result->id = "{}";
    vertex_result->production_root = false;
    cout << "New node ASG with id {}" << endl;
    vertex_result->vertex_to.push_back(vertex_byExpression(productionterm0)); //vertice 1
    return vertex_result;
  } else {
    cout << "ELSE FACTOR  " << endl;
    string attributes;
    unsigned int del_first;
    unsigned int del_last;

    del_first = expression.find("<.");
    del_last = expression.find(".>");

    if(del_first < expression.size() && del_last < expression.size()){
      attributes = expression.substr(del_first, del_last-del_first+2);
      expression.erase(del_first, del_last-del_first+2);
      cout << "Attributes " << attributes << endl;
    }

    possible_production_name = expression;

    auto lambda = [production_name](const vertex_asg* current) {
      return current->id == production_name;
    };

    vector< vertex_asg* >::iterator production_name_it = find_if(begin(productions_root), end(productions_root), lambda);
    if(production_name_it != end(productions_root)){
      //cout << "FOUND " << (*production_name_it)->id <<  endl;
      return (*production_name_it);
    }else{
      vertex_asg* vertex_result = new vertex_asg;
      vertex_result->id = expression;
      vertex_result->production_root = false;
      cout << "New node ASG with id " << expression << endl;
      return vertex_result;
    };
  }
}
