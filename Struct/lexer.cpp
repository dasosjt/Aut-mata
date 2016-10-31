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
AFD* Lexer::constr_AFD;
Tree* Lexer::constr_tree;
vector<char> Lexer::constr_lang;
unordered_map<string,string> Lexer::symbol_table;

Lexer::Lexer(const char* file_name){
  this->file_name = file_name;
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
  endof_line = char(10);
  whitespace = open_brackets+char(32)+close_brackets;
  this->letter = open_paranthesis+"a"+point+point+"z"+or_operation+"A"+point+point+"Z"+close_paranthesis;
  this->digit = open_paranthesis+"0"+point+point+"9"+close_paranthesis;
  this->ident = open_paranthesis+this->letter+open_brackets+this->letter+or_operation+this->digit+close_brackets+close_paranthesis;
  this->number = this->digit+open_brackets+this->digit+close_brackets;
  anything = open_paranthesis+open_brackets+this->letter+or_operation+this->ident+or_operation+this->number+or_operation+quote+or_operation+apostrophe+or_operation+endof_line+or_operation+whitespace+close_brackets+close_paranthesis;
  this->constr = open_paranthesis+"COMPILER"+whitespace+this->ident+whitespace+endof_line+"CHARACTERS"+whitespace+endof_line+anything+"KEYWORDS"+whitespace+endof_line+anything+"TOKENS"+whitespace+endof_line+anything+"PRODUCTIONS"+whitespace+endof_line+"END"+whitespace+this->ident+endof_line+close_paranthesis;
  ident_tree = new Tree();
  ident_tree->parse(this->ident);
  ident_lang = ident_tree->getL();
  ident_AFD = new AFD("ident.txt", "ident_cocol");
  ident_AFD->createAFD(ident_tree->getRoot(), ident_lang);
  number_tree = new Tree();
  number_tree->parse(this->number);
  number_lang = number_tree->getL();
  number_AFD = new AFD("number.txt", "number_cocol");
  number_AFD->createAFD(number_tree->getRoot(), number_lang);
  /*constr_tree = new Tree();
  constr_tree->parse(this->constr);
  constr_lang = constr_tree->getL();
  constr_AFD = new AFD();
  constr_AFD->createAFD(constr_tree->getRoot(), constr_lang);*/
}

void Lexer::Parse(){
  file.open(file_name);
  if(file.is_open()){
    file_contents = {istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
    //file_contents.erase(remove(file_contents.begin(), file_contents.end(), char(10)), file_contents.end());
    cout << file_contents << endl;
    file.close();
  }
  /*constr_AFD->simulationAFD(file_contents);*/
}

void Lexer::cout_symbol_table(){
  cout << endl;
  cout << "Current Ident: " << current_ident << endl;
  cout << "Type Decl: " << typeDecl << endl;
  cout << "symbol_table: " << endl;
  for (auto it = symbol_table.begin(); it != symbol_table.end(); ++it){
      cout << " " << it->first << " : " << it->second << endl;
  }
  cout << endl;
}

void Lexer::add_symbol_table(string symbol_to_append){
  cout << "Current Ident: " << current_ident << endl;
  cout << "Type Decl: " << typeDecl << endl;
  cout << "Symbol to append: " << symbol_to_append << endl;
  if(typeDecl != "token"){
    symbol_table[current_ident]+=symbol_to_append;
  }
  cout_symbol_table();
}

void Lexer::symbol_to_AFN(){
  for (auto it = symbol_table.begin(); it != symbol_table.end(); ++it){
    Tree* primitive0_tree = new Tree();
    primitive0_tree->parse(it->second);
    vector<char > primitive0_lang = primitive0_tree->getL();
    AFD* primitive0_AFD = new AFD(it->first+".txt", it->first);
    primitive0_AFD->createAFD(primitive0_tree->getRoot(), primitive0_lang);
    primitives_AFD.push_back(primitive0_AFD);
    cout << "AFD done! " << it->first << " : " << it->second << endl;
  }
  primitives_AFD[0]->simulationAFD("lalalalal000000");
}

bool Lexer::SetDecl(string expression){
  cout << "SetDecl" << endl;
  typeDecl = "set";
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
      if(Ident(ident)){
        current_ident = ident;
        if(Set(set)){
          return true;
        }
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
  bool simulationAFD = ident_AFD->simulationAFD(expression);
  if(simulationAFD){
    if(typeDecl != "token"){
      auto found = symbol_table.find(expression);
      if(found == symbol_table.end()){
          cout << "Not found " << expression << " in the symbol table" << endl;
      }else{
        cout << "Found " << found->first << " is " << found->second << endl;
        add_symbol_table(signToSpecialChar("(")+found->second+signToSpecialChar(")"));
      }
    }else{

    }
  }
  return simulationAFD;
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
      if(BasicSet(basicset0)){
        string mp = expression.substr(delimeter,1);
        if(mp == "+"){
          add_symbol_table(mp);
        }
        if(Set(basicset1)){
          return true;
        }
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
  if(n%2 == 0 && n >= 2){
    cout << "YES" << endl;
    expression = expression.substr(1, expression.size() - 2);
    if(typeDecl == "set"){
      unsigned int n = expression.size()-1;
      for(unsigned int i = 0; i<n; i++){
        expression.insert(2*i+1, signToSpecialChar("|"));
      }
    }else{
      current_pbb_signs.push("p");
    }
    add_symbol_table(expression);
    return true;
  }
  cout << "NO" << endl;
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
    cout << "YES" << endl;
    add_symbol_table(expression);
    return true;
  } else {
    unsigned int pp = expression.find("CHR(");
    if(pp < expression.size()){
      unsigned int p1 = expression.find("(");
      unsigned int p2 = expression.find(")");
      string number = expression.substr(p1+1, expression.size()-p2);
      if(Number(number)){
        cout << "YES" << endl;
        add_symbol_table(expression);
        return true;
      }
    }
  }
  return false;
}

bool Lexer::KeywordDecl(string expression){
  cout << "KeywordDecl" << endl;
  typeDecl = "keyword";
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
      if(Ident(ident)){
        current_ident = ident;
        if(String(str)){
           return true;
        }
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
  typeDecl = "token";
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
      if(Ident(ident)){
        current_ident = ident;
        if(TokenExpr(tokenexpr)){
          string hash_expression = tokenToHash(tokenexpr);
            cout << hash_expression << endl;
            typeDecl = "";
            add_symbol_table(hash_expression);
            return true;
        }
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
  int quote_mark = 0;
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
    } else if(expression.at(i) == '\"'){
      quote_mark += 1;
    }
    i++;
  }
  while(!pbb_signs.empty()){
    pbb_signs.pop();
  }
  if(!tokenterm1.empty() && !tokenterm0.empty()){
    if(TokenTerm(tokenterm0)){
      if(TokenExpr(tokenterm1)){
          return true;
      }
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
  int quote_mark = 0;
  tokenfactor0 = expression;
  unsigned int i = 0;
  bool stop = false;
  while(i<expression.size() && !stop){
    if(expression.at(i) == '(' || expression.at(i) == '{' || expression.at(i) == '['){
      if(i>0 && pbb_signs.empty() && quote_mark%2 == 0){
        tokenfactor0 = expression.substr(0, i);
        tokenfactor1 = expression.substr(i, expression.size()-i);
        stop = true;
      }
      pbb_signs.push(expression.at(i));
    } else if(expression.at(i) == ')' || expression.at(i) == '}' || expression.at(i) == ']'){
      pbb_signs.pop();
      if(i>0 && pbb_signs.empty() && quote_mark%2 == 0){
        tokenfactor0 = expression.substr(0, i+1);
        tokenfactor1 = expression.substr(i+1, expression.size()-i-1);
        stop = true;
      }
    } else if(expression.at(i) == '\"'){
      quote_mark += 1;
      if(i>0 && pbb_signs.empty() && quote_mark%2 == 0){
        tokenfactor0 = expression.substr(0, i+1);
        tokenfactor1 = expression.substr(i+1, expression.size()-i-1);
        stop = true;
      }
    }
    i++;
  }
  while(!pbb_signs.empty()){
    pbb_signs.pop();
  }
  if(!tokenfactor0.empty() && !tokenfactor1.empty()){
    if(TokenFactor(tokenfactor0) && TokenTerm(tokenfactor1)){
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
    if(Symbol(expression)){
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

string Lexer::signToSpecialChar(string r0){
  if(r0 == "("){
    r0 = open_paranthesis;
  }else if(r0 == ")"){
    r0 = close_paranthesis;
  }if(r0 == "{"){
    r0 = open_brackets;
  }else if(r0 == "}"){
    r0 = close_brackets;
  }if(r0 == "["){
    r0 = open_braces;
  }else if(r0 == "]"){
    r0 = close_braces;
  }else if(r0 == "|"){
    r0 = or_operation;
  }
  return r0;
}

string Lexer::tokenToHash(string expression){
  string result;
  string r0;
  string r1;
  unsigned int delimeter = expression.find_first_of("{}()[]]|\"");
  if(delimeter == 0){
    //signo
    r0 = expression.substr(0, 1);
    r0 = signToSpecialChar(r0);
    if(r0 == "\""){
      r0 = "";
    }

    //expression
    r1 = expression.substr(1,expression.size());
    auto found = symbol_table.find(r1);
    if(found != symbol_table.end()){
        r1 = signToSpecialChar("(")+found->second+signToSpecialChar(")");
    }else{
      r1 = signToSpecialChar(r1);
    }

  }else if(delimeter<expression.size()){
    //expression fijo!
    r0 = expression.substr(0, delimeter);
    if(r0 == "\""){
      r0 = "";
    }
    auto found = symbol_table.find(r0);
    if(found != symbol_table.end()){
        r0 = signToSpecialChar("(")+found->second+signToSpecialChar(")");
    }else{
      r0 = signToSpecialChar(r0);
    }

    //expression
    r1 = expression.substr(delimeter, expression.size()-delimeter);
    found = symbol_table.find(r1);
    if(found != symbol_table.end()){
        r1 = signToSpecialChar("(")+found->second+signToSpecialChar(")");
    }else{
      r1 = signToSpecialChar(r1);
    }
  }
  //cout << "R0 " << r0 << endl;
  //cout << "R1 " << r1 << endl;
  if(!r1.empty()){
      result = r0 + tokenToHash(r1);
  }else{
    result = r0;
  }
  return result;
}
