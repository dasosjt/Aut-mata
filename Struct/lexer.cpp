#include "lexer.h"
#include "string"


Lexer::Lexer(const char* file_name){
  this->file_name = file_name;
  this->letter = "a..z";
  this->digit = "0|1|2|3|4|5|6|7|8|9";
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
  if(expression.at(expression.size()-1) == '.'){
    unsigned int delimeter = expression.find('=');
    string ident;
    string set;
    if(delimeter != string::npos){
      ident = expression.substr(0, delimeter);
      set = expression.substr(delimeter, expression.size());
      if(Ident(ident) && Set(set)){
          return true;
      }
    }
    return false;
  } else {
    cout << "fuckin.. punto" << endl;
    return false;
  }
}

bool Lexer::Ident(string expression){
  return true;
}

bool Lexer::Set(string expression){
  // falta verificar el kleene
  unsigned int delimeterp = expression.find('=');
  unsigned int delimeterm = expression.find('=');
  string basicset0;
  string basicset1;
  if (delimeterp != string::npos && delimeterm == string::npos){
    basicset0 = expression.substr(0, delimeterp);
    basicset1 = expression.substr(delimeterp, expression.size());
    if(BasicSet(basicset0) && BasicSet(basicset1)){
      return true;
    }
  } else if(delimeterm != string::npos && delimeterp == string::npos){
    basicset0 = expression.substr(0, delimeterm);
    basicset1 = expression.substr(delimeterm, expression.size());
    if(BasicSet(basicset0) && BasicSet(basicset1)){
      return true;
    }
  } else if(BasicSet(expression)){
    return true;
  }
  return false;
}

bool Lexer::BasicSet(string expression){
  // revisar los ..
  if(String(expression) || Ident(expression) || Char(expression)){
    return true;
  }
}

bool Lexer::String(string expression{
  return true;
}

bool Lexer::Char(string expression){
  return true;
}

bool Lexer::KeywordDecl(string expression){
  if(expression.at(expression.size()-1) == '.'){
    unsigned int delimeter = expression.find('=');
    string ident;
    string str;
    if(delimeter != string::npos){
      ident = expression.substr(0, delimeter);
      str = expression.substr(delimeter, expression.size());
      if(Ident(ident) && String(set)){
          return true;
      }
    }
  } else {
    cout << "fuckin... punto" << endl;
  }
  return false;
}

bool Lexer::TokenDecl(string expression){
  if(expression.at(expression.size()-1) == '.'){
    unsigned int delimeter = expression.find('=');
    string ident;
    string tokenexpr;
    if(delimeter != string::npos){
      ident = expression.substr(0, delimeter);
      tokenexpr = expression.substr(delimeter, expression.size());
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
  //falta agregar el kleene
}

bool Lexer::TokenFactor(string expression){
  unsigned int parentheses = expression.find('(');
  unsigned int braces = expression.find('{');
  unsigned int brackets = expression.find('[');
  if(parentheses != string::npos && braces == string::npos && brackets == string::npos){
    return TokenExpr(expression.substr(1, expression.size()-1));
  } else if(brackets != string::npos && parentheses == string::npos && braces == string::npos){
    return TokenExpr(expression.substr(1, expression.size()-1));
  } else if(braces != string::npos && brackets == string::npos && parentheses == string::npos){
    return TokenExpr(expression.substr(1, expression.size()-1));
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
