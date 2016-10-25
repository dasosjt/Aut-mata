#include "lexer.h"
#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include "string"
#include <algorithm>
#include <iterator>
#include <unordered_map>

AFD* Lexer::ident_AFD;
Tree* Lexer::ident_tree;
vector<char> Lexer::ident_lang;
AFD* Lexer::number_AFD;
Tree* Lexer::number_tree;
vector<char> Lexer::number_lang;
unordered_map<string,string> Lexer::symbol_table;

Lexer::Lexer(const char* file_name){
  point = char(176);
  or_operation = char(179);
  open_paranthesis = char(244);
  close_paranthesis = char(245);
  open_braces = char(191);
  close_braces =  char(192);
  open_brackets = char(212);
  close_brackets = char(213);
  quote = char(34);
  apostrophe = char(39);
  this->file_name = file_name;
  this->letter = open_paranthesis+"a"+point+point+"z"+or_operation+"A"+point+point+"Z"+close_paranthesis;
  this->digit = open_paranthesis+"0"+point+point+"9"+close_paranthesis;
  this->ident = open_paranthesis+this->letter+open_brackets+this->letter+or_operation+this->digit+close_brackets+close_paranthesis;
  this->number = this->digit+open_brackets+this->digit+close_brackets;
  ident_tree = new Tree();
  ident_tree->parse(this->ident);
  ident_lang = ident_tree->getL();
  ident_AFD = new AFD();
  ident_AFD->createAFD(ident_tree->getRoot(), ident_lang);
  number_tree = new Tree();
  number_tree->parse(this->number);
  number_lang = number_tree->getL();
  number_AFD = new AFD();
  number_AFD->createAFD(number_tree->getRoot(), number_lang);
}

void Lexer::Parse(){
  file.open(file_name);
  string line;
  int number_line;
  if(file.is_open()){
    number_line = 1;
    while(getline(file, line)){
      cout << number_line << " " << line << endl;
      number_line ++;
    }
    file.close();
  }
}

void Lexer::cout_symbol_table(){
  cout << endl;
  cout << "symbol_table: " << endl;
  for ( auto it = symbol_table.begin(); it != symbol_table.end(); ++it )
  cout << " " << it->first << " : " << it->second << endl;
  cout << endl;
}

bool Lexer::SetDecl(string expression){
  cout << "SetDecl" << endl;
  expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
  if(expression.at(expression.size()-1) == '.'){
    expression.pop_back();
    cout << "found ." << endl;
    unsigned int delimeter = expression.find('=');
    string ident;
    string set;
    if(delimeter < expression.size()){
      cout << "found =" << endl;
      ident = expression.substr(0, delimeter);
      set = expression.substr(delimeter+1, expression.size()-delimeter-1);
      if(Ident(ident) && Set(set)){
        symbol_table[ident]=set;
        cout_symbol_table();
        return true;
      }
    } else {
      cout << "fuckin.. = " << endl;
    }
  } else {
    cout << "fuckin.. punto" << endl;
  }
  return false;
}

bool Lexer::Ident(string expression){
  cout << "Ident " << expression << endl;
  return ident_AFD->simulationAFD(expression);
}

bool Lexer::Set(string expression){
  unsigned int np = count(expression.begin(), expression.end(), '+');
  unsigned int mp = count(expression.begin(), expression.end(), '-');
  cout << "Set " << expression << endl;
  if(np > 0 || mp > 0){
    unsigned int delimeter = expression.find_first_of("+-");
    string basicset0;
    string basicset1;
    if(delimeter < expression.size()){
      basicset0 = expression.substr(0, delimeter);
      basicset1 = expression.substr(delimeter+1, expression.size()-delimeter-1);
      cout << basicset0 << endl;
      cout << basicset1 << endl;
      if(BasicSet(basicset0) && Set(basicset1)){
        return true;
      }
    }
  } else if(BasicSet(expression)){
    return true;
  }
  return false;
}

bool Lexer::BasicSet(string expression){
  // revisar los ..
  cout << "BasicSet " << expression << endl;
  unsigned int pp = expression.find("..");
  if(pp > expression.size()){
    if(String(expression) || Ident(expression) || Char(expression)){
      return true;
    }
  } else {
    string char0;
    string char1;
    char0 = expression.substr(0, pp);
    char1 = expression.substr(pp+2, expression.size()-pp-2);
    cout << char0 << endl;
    cout << char1 << endl;
    if(Char(char0) && Char(char1)){
      return true;
    }
  }
  return false;
}

bool Lexer::String(string expression){
  unsigned int n = count(expression.begin(), expression.end(), '"');
  cout << "String " << expression << endl;
  if(n==2){
    return true;
  }
  return false;
}

bool Lexer::Number(string expression){
  cout << "Number " << expression << endl;
  return number_AFD->simulationAFD(expression);
}

bool Lexer::Char(string expression){
  unsigned int n = count(expression.begin(), expression.end(), '\'');
  if(n==2){
    cout << "Char " << expression << endl;
    return true;
  } else {
    unsigned int pp = expression.find("CHR(");
    if(pp < expression.size()){
      unsigned int p1 = expression.find("(");
      unsigned int p2 = expression.find(")");
      string number = expression.substr(p1+1, expression.size()-p2);
      if(Number(number)){
        return true;
      }
    }
  }
  return false;
}

bool Lexer::KeywordDecl(string expression){
  cout << "KeywordDecl" << endl;
  expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
  if(expression.at(expression.size()-1) == '.'){
    expression.pop_back();
    unsigned int delimeter = expression.find('=');
    string ident;
    string str;
    if(delimeter < expression.size()){
      ident = expression.substr(0, delimeter);
      str = expression.substr(delimeter+1, expression.size()-delimeter-1);
      cout << ident << endl;
      cout << str << endl;
      if(Ident(ident) && String(str)){
          return true;
      }
    } else {
        cout << "fuckin... =" << endl;
    }
  } else {
    cout << "fuckin... punto" << endl;
  }
  return false;
}

bool Lexer::TokenDecl(string expression){
  cout << "TokenDecl" << endl;
  expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
  if(expression.at(expression.size()-1) == '.'){
    expression.pop_back();
    cout << "found ." << endl;
    unsigned int delimeter = expression.find('=');
    string ident;
    string tokenexpr;
    if(delimeter < expression.size()){
      cout << "found =" << endl;
      ident = expression.substr(0, delimeter);
      tokenexpr = expression.substr(delimeter+1, expression.size()-delimeter-1);
      cout << "Ident " << ident << endl;
      cout << "Token expression " << tokenexpr << endl;
      if(Ident(ident) && TokenExpr(tokenexpr)){
        return true;
      }
    } else {
      if(Ident(expression)){
        return true;
      }
    }
  } else {
    cout << "fuckin... punto" << endl;
  }
  return false;
}

bool Lexer::TokenExpr(string expression){
  cout << "TokenExpression " << expression << endl;
  string tokenterm0;
  string tokenterm1;
  unsigned int i = 0;
  bool stop = false;
  while(i<expression.size() && !stop){
    if(expression.at(i) == '|' and pbb_signs.empty()){
      tokenterm0 = expression.substr(0, i);
      tokenterm1 = expression.substr(i+1, expression.size()-i-1);
      stop = true;
    }
    if(expression.at(i) == '(' || expression.at(i) == '{' || expression.at(i) == '['){
      pbb_signs.push(expression.at(i));
    } else if(expression.at(i) == ')' || expression.at(i) == '}' || expression.at(i) == ']'){
      pbb_signs.pop();
    }
    i++;
  }
  while(!pbb_signs.empty()){
    pbb_signs.pop();
  }
  if(!tokenterm1.empty() && !tokenterm0.empty()){
    if(TokenTerm(tokenterm0) && TokenTerm(tokenterm1)){
      return true;
    }
  }else if(!tokenterm0.empty()){
    if(TokenTerm(tokenterm0)){
      return true;
    }
  }else{
    if(TokenTerm(expression)){
      return true;
    }
  }
  return false;
}

bool Lexer::TokenTerm(string expression){
  cout << "TokenTerm " << expression << endl;
  string tokenfactor0;
  string tokenfactor1;
  tokenfactor0 = expression;
  unsigned int i = 0;
  bool stop = false;
  while(i<expression.size() && !stop){
    if(expression.at(i) == '(' || expression.at(i) == '{' || expression.at(i) == '['){
      if(i>0 && pbb_signs.empty()){
        tokenfactor0 = expression.substr(0, i);
        tokenfactor1 = expression.substr(i, expression.size()-i);
        //cout << tokenfactor0 << endl;
        //cout << tokenfactor1 << endl;
        stop = true;
      }
      pbb_signs.push(expression.at(i));
    } else if(expression.at(i) == ')' || expression.at(i) == '}' || expression.at(i) == ']'){
      pbb_signs.pop();
    }
    i++;
  }
  while(!pbb_signs.empty()){
    pbb_signs.pop();
  }
  if(!tokenfactor0.empty() && !tokenfactor1.empty()){
    if(TokenFactor(tokenfactor0) && TokenFactor(tokenfactor1)){
      return true;
    }
  }else{
    if(TokenFactor(tokenfactor0)){
      return true;
    }
  }
  return false;
}

bool Lexer::TokenFactor(string expression){
  cout << "TokenFactor " << expression << endl;
  string tokenterm0;
  tokenterm0 = expression.substr(1, expression.size()-2);
  if(expression.at(0) == '(' && expression.at(expression.size()-1) == ')'){
    if(TokenExpr(tokenterm0)){
        return true;
    }
  } else if(expression.at(0) == '[' && expression.at(expression.size()-1) == ']'){
    if(TokenExpr(tokenterm0)){
        return true;
    }
  } else if(expression.at(0) == '{' && expression.at(expression.size()-1) == '}'){
    if(TokenExpr(tokenterm0)){
        return true;
    }
  } else {
    tokenterm0 = expression;
    if(Symbol(tokenterm0)){
      return true;
    }
  }
  return false;
}

bool Lexer::Symbol(string expression){
  if(Ident(expression) || String(expression) || Char(expression)){
    return true;
  }
  return false;
}
