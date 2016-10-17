#include "lexer.h"
#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include "string"
#include <algorithm>
#include <iterator>

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
  cout << apostrophe << endl;
  this->file_name = file_name;
  this->letter = open_paranthesis+"a"+point+point+"z"+close_paranthesis;
  this->digit = open_paranthesis+"0"+point+point+"9"+close_paranthesis;
  this->ident = open_paranthesis+this->letter+open_brackets+this->letter+or_operation+this->digit+close_brackets+close_paranthesis;
  this->number = this->digit+open_brackets+this->digit+close_brackets;
  this->char_ = open_paranthesis+"CHR("+open_paranthesis+this->number+close_paranthesis+")"+close_paranthesis;
  this->character = open_paranthesis+apostrophe+open_paranthesis+this->letter+or_operation+this->number+close_paranthesis+apostrophe+close_paranthesis;
  this->character_ = open_paranthesis+char_+or_operation+character+close_paranthesis;
  this->string_ = quote+this->letter+open_brackets+this->letter+close_brackets+quote;
  this->basicSet = open_paranthesis+open_paranthesis+char_+open_braces+".."+char_+close_braces+close_paranthesis+or_operation+ident+or_operation+string_+close_paranthesis;
  this->set = open_paranthesis+basicSet+open_brackets+open_paranthesis+"+"+or_operation+"-"+close_paranthesis+basicSet+close_brackets+close_paranthesis;
  this->setDecl = ident+"="+set;
  string exprsn;
  vector<char> L;
  Tree* tree = new Tree();
  tree->parse(character_);
  cout << "AST " << endl;
  tree->display();
  L = tree->getL();//aaddasefvaawevGGFEWQ2
  /*cout << "AFN START" << endl;
  AFN* afn = new AFN();
  afn->createAFN(tree->getRoot(), L);
  cout << "AFN READY FOR TEST" << endl;
  cout << "Enter Expression " << endl;
  cin >> exprsn ;
  afn->simulationAFN(exprsn);*/
  cout << "AST TO AFD "<< endl;
  AFD* afd = new AFD();
  afd->createAFD(tree->getRoot(), L);
  cout << "AFD READY FOR TEST" << endl;
  cout << "Enter Expression " << endl;
  cin >> exprsn ;
  afd->simulationAFD(exprsn);
  afd->minAFD();
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

bool Lexer::SetDecl(string expression){
  cout << "SetDecl" << endl;
  expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
  if(expression.at(expression.size()-1) == '.'){
    expression.pop_back();
    //cout << "found ." << endl;
    unsigned int delimeter = expression.find('=');
    string ident;
    string set;
    if(delimeter < expression.size()){
      //cout << "found =" << endl;
      ident = expression.substr(0, delimeter);
      set = expression.substr(delimeter+1, expression.size());
      if(Ident(ident) && Set(set)){
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
  cout << "ident " << expression << endl;
  return true;
}

bool Lexer::Set(string expression){
  unsigned int np = count(expression.begin(), expression.end(), '+');
  unsigned int mp = count(expression.begin(), expression.end(), '-');
  if(np > 0 || mp > 0){
    unsigned int delimeter = expression.find_first_of("+-");
    string basicset0;
    string basicset1;
    cout << "Set " << expression << endl;
    if(delimeter < expression.size()){
      basicset0 = expression.substr(0, delimeter);
      basicset1 = expression.substr(delimeter+1, expression.size());
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
  unsigned int pp = expression.find("..");
  if(pp < expression.size()){
    if(String(expression)){
      return true;
    }
  } else {
    if(String(expression) || Ident(expression) || Char(expression)){
      return true;
    }
  }
  return false;
}

bool Lexer::String(string expression){
  unsigned int n = count(expression.begin(), expression.end(), '"');
  if(n==0){
    return true;
  }
  return false;
}

bool Lexer::Number(string expression){
  return true;
}

bool Lexer::Char(string expression){
  unsigned int n = count(expression.begin(), expression.end(), '\'');
  if(n==0){
    return true;
  } else {
    unsigned int pp = expression.find("CHR(");
    if(pp < expression.size()){
      unsigned int p1 = expression.find("(");
      unsigned int p2 = expression.find(")");
      string number = expression.substr(p1+1, p2-1);
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
    if(delimeter < string::npos){
      ident = expression.substr(0, delimeter);
      str = expression.substr(delimeter, expression.size());
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
      tokenexpr = expression.substr(delimeter+1, expression.size());
      cout << ident << endl;
      cout << tokenexpr << endl;
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
  unsigned int nor = count(expression.begin(), expression.end(), '|');
  if(nor > 0){
    unsigned int delimeter = expression.find_first_of("|");
    string tokenterm0;
    string tokenterm1;
    cout << "TokenExpr " << expression << endl;
    if(delimeter < expression.size()){
      tokenterm0 = expression.substr(0, delimeter);
      tokenterm1 = expression.substr(delimeter+1, expression.size());
      cout << tokenterm0 << endl;
      cout << tokenterm1 << endl;
      if(TokenTerm(tokenterm0) && TokenExpr(tokenterm1)){
        return true;
      }
    }
  } else if(TokenTerm(expression)){
    return true;
  }
  return false;
}

bool Lexer::TokenTerm(string expression){
  unsigned int delimeter = expression.find_first_of(")]}");
  string tokenfactor0;
  string tokenfactor1;
  cout << "TokenTerm " << expression << endl;
  if(delimeter < expression.size()){
    cout << delimeter << endl;
    cout << expression.size() << endl;
    if(delimeter == expression.size() -1){
      cout << "Just one token" << endl;
      if(TokenFactor(expression)){
        return true;
      }
    } else {
      cout << "Well.. multiple token" << endl;
      tokenfactor0 = expression.substr(0, delimeter);
      tokenfactor1 = expression.substr(delimeter, expression.size());
      cout << tokenfactor0 << endl;
      cout << tokenfactor1 << endl;
      if(TokenFactor(tokenfactor0) && TokenFactor(tokenfactor1)){
        return true;
      }
    }
  } else {
    if(TokenFactor(expression)){
      return true;
    }
  }
  return false;
}

bool Lexer::TokenFactor(string expression){
  unsigned int parentheses = expression.find('(');
  unsigned int braces = expression.find('{');
  unsigned int brackets = expression.find('[');
  cout << "TokenFactor " << expression << endl;
  if(parentheses < expression.size() && braces > expression.size() && brackets > expression.size()){
    expression.pop_back();
    expression.erase(0,1);
    cout << expression << endl;
    return TokenExpr(expression);
  } else if(parentheses > expression.size() && braces > expression.size() && brackets < expression.size()){
    expression.pop_back();
    expression.erase(0,1);
    cout << expression << endl;
    return TokenExpr(expression);
  } else if(parentheses > expression.size() && braces < expression.size() && brackets > expression.size()){
    expression.pop_back();
    expression.erase(0,1);
    cout << expression << endl;
    return TokenExpr(expression);
  } else {
    return Symbol(expression);
  }
  return false;
}

bool Lexer::Symbol(string expression){
  if(Ident(expression) || String(expression) || Char(expression)){
    return true;
  }
  return false;
}
