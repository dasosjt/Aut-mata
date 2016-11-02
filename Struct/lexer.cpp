#include "lexer.h"
#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include "string"
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <sstream>

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
unordered_map<string,string> Lexer::type_table;

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
  ident_tree = new Tree();
  ident_tree->parse(this->ident);
  ident_lang = ident_tree->getL();
  ident_AFD = new AFD("ident_cocol.txt", "ident_cocol");
  ident_AFD->createAFD(ident_tree->getRoot(), ident_lang);
  number_tree = new Tree();
  number_tree->parse(this->number);
  number_lang = number_tree->getL();
  number_AFD = new AFD("number_cocol.txt", "number_cocol");
  number_AFD->createAFD(number_tree->getRoot(), number_lang);
}

void Lexer::Parse(){
  file.open(file_name);
  if(file.is_open()){
    file_contents = {istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
    file.close();
  }
  unsigned int compiler_pos = file_contents.find("COMPILER ");
  unsigned int characters_pos = file_contents.find("CHARACTERS");
  unsigned int keywords_pos = file_contents.find("KEYWORDS");
  unsigned int tokens_pos = file_contents.find("TOKENS");
  unsigned int productions_pos = file_contents.find("PRODUCTIONS");
  unsigned int end_pos = file_contents.find("END ");
  string segment;
  bool error = false;
  if(characters_pos<file_contents.size() && keywords_pos<file_contents.size()){
      stringstream characters(file_contents.substr(characters_pos+11, keywords_pos-characters_pos-11));
      while(getline(characters, segment, char(10))){
        error = SetDecl(segment);
      }
  }
  if(keywords_pos<file_contents.size() && tokens_pos<file_contents.size()){
      stringstream keywords(file_contents.substr(keywords_pos+9, tokens_pos-keywords_pos-9));
      while(getline(keywords, segment, char(10))){
        error = KeywordDecl(segment);
      }
  }
  if(tokens_pos<file_contents.size() && productions_pos<file_contents.size()){
      stringstream tokens(file_contents.substr(tokens_pos+7, productions_pos-tokens_pos-7));
      while(getline(tokens, segment, char(10))){
        error = TokenDecl(segment);
      }
  }
  if(end_pos<file_contents.size()){
      string end = file_contents.substr(end_pos+4, file_contents.size()-end_pos-4);
  }
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

void Lexer::cout_type_table(){
  cout << endl;
  cout << "type_table: " << endl;
  for (auto it = type_table.begin(); it != type_table.end(); ++it){
      cout << " " << it->first << " : " << it->second << endl;
  }
  cout << endl;
}

void Lexer::add_type_table(string ident){
  type_table[ident] = typeDecl;
  cout_type_table();
}

void Lexer::symbol_to_AFN(){
  Tree* primitive0_tree;
  vector<char > L;
  AFD* primitive0_AFD;
  for (auto it = symbol_table.begin(); it != symbol_table.end(); ++it){
    vector<char > primitive0_lang;
    primitive0_tree = new Tree();
    primitive0_tree->parse(it->second);
    primitive0_lang = primitive0_tree->getL();
    auto found = type_table.find(it->first);
    if(found->second != "set"){
        primitive0_AFD = new AFD(""+it->first+".txt", found->second+"-"+it->first);
        primitive0_AFD->createAFD(primitive0_tree->getRoot(), primitive0_lang);
        primitives_AFD.push_back(primitive0_AFD);
        cout << "AFD done! " << it->first << " : " << it->second << endl;
        L.insert(L.end(), primitive0_lang.begin(), primitive0_lang.end());
        sort(L.begin(), L.end());
        L.erase(unique(L.begin(),L.end() ),L.end());
    }
    primitive0_lang.clear();
  }
  AFN* big_one = new AFN();
  big_one->newfi_vertex(primitive0_AFD->get_state());
  cout << big_one->get_state() << endl;
  copy(L.begin(), L.end(), ostream_iterator<char>(cout, " "));
  cout << endl;
  for(AFD* current : primitives_AFD){
    big_one->get_vertex_init()->vertex_to.push_back(make_pair(char(238), current->get_vertex_init()));
    for(vertex* v : current->get_allvertex_final()){
      v->vertex_to.push_back(make_pair(char(238), big_one->get_vertex_final()));
    }
  }
  cout << endl;
  big_one->setL(L);
  string exprsn;
  big_one->mapAFN(big_one->get_vertex_init());
  big_one->writeToFile("scanner.cpp");
  /*while(true){
    cout << "SIMULATE BIG AFN with ? " << endl;
    cin >> exprsn;
    cout << endl;
    big_one->simulationAFN(exprsn);
  }*/
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
        add_type_table(ident);
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
  typeDecl = "set";
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
          add_symbol_table(or_operation);
        }else{
          //auto found = symbol_table.find(current_ident);
          //cout <<" Minus XXXXXXXXXXXXXXXXXXXXXXXXXXX " << found->second << endl;
          add_symbol_table("-");
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
        //expression.insert(2*i+1,1,'|');
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
        add_type_table(ident);
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
        add_type_table(ident);
        if(TokenExpr(tokenexpr)){
          string hash_expression = tokenToHash(tokenexpr);
          printExpression(hash_expression);
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
  }else if(r0 == "{"){
    r0 = open_brackets;
  }else if(r0 == "}"){
    r0 = close_brackets;
  }else if(r0 == "["){
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
    r0 = expression.substr(0,1);
    auto found = symbol_table.find(r1);
    if(found != symbol_table.end()){
        r0 = signToSpecialChar("(")+found->second+signToSpecialChar(")");
        //r1 = "("+found->second+")";
    }else{
      r0 = signToSpecialChar(r0);
    }

    if(r0 == "\""){
      r0 = "";
    }

    //expression
    r1 = expression.substr(1,expression.size());
    found = symbol_table.find(r1);
    if(found != symbol_table.end()){
        r1 = signToSpecialChar("(")+found->second+signToSpecialChar(")");
        //r1 = "("+found->second+")";
    }else{
      r1 = signToSpecialChar(r1);
    }

  }else if(delimeter<expression.size()){
    //expression fijo!
    r0 = expression.substr(0, delimeter);
    auto found = symbol_table.find(r0);
    if(found != symbol_table.end()){
        r0 = signToSpecialChar("(")+found->second+signToSpecialChar(")");
        //r0 = "("+found->second+")";
    }else{
      r0 = signToSpecialChar(r0);
    }
    if(r0 == "\""){
      r0 = "";
    }

    //expression
    r1 = expression.substr(delimeter, expression.size()-delimeter);
    found = symbol_table.find(r1);
    if(found != symbol_table.end()){
        r1 = signToSpecialChar("(")+found->second+signToSpecialChar(")");
        //r1 = "("+found->second+")";
    }else{
      r1 = signToSpecialChar(r1);
    }
  }else if(delimeter != 0){
    //cout << "HEY IM LOST :(" << endl;
    return expression;
  }
  //cout << "R0 ";
  printExpression(r0);
  //cout << "R1 ";
  printExpression(r1);
  if(!r1.empty() && !r0.empty()){
    //cout << "R0 and R1" << endl;
    result = r0 + tokenToHash(r1);
  }else if(!r0.empty()){
    //cout << "R0" << endl;
    result = tokenToHash(r0);
  }else if(!r1.empty()){
    //cout << "R1" << endl;
    result = tokenToHash(r1);
  }
  return result;
}


void Lexer::printExpression(string exprsn){
  for(auto i: exprsn){
    if(i == char(167)){
      cout << '^';
    }else if(i == char(179)){
      cout << '|';
    }else if(i == char(241)){
      cout << '*';
    }else if(i == char(238)){
      cout << "epsilon";
    }else if(i == char(212)){
      cout << "{";
    }else if(i == char(213)){
      cout << "}";
    }else if(i == char(244)){
      cout << "(";
    }else if(i == char(245)){
      cout << ")";
    }else{
     cout << i;
    }
  }
  cout << endl;
}
