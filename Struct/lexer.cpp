#include "lexer.h"
#include "string"
#include <algorithm>
#include <iterator>

Lexer::Lexer(const char* file_name){
  this->file_name = file_name;
  this->letter = "(a..z)";
  this->digit = "(0|1|2|3|4|5|6|7|8|9)";
  this->ident = this->letter+"{"+this->letter+"|"+this->digit+"}";
  this->number = this->digit+"{"+this->digit+"}";
  cout << ident << endl;
  cout << number << endl;

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
