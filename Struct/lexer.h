#ifndef LEXER_H
#define LEXER_H

#include "lexer.h"
#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include <iostream>
#include <fstream>

using namespace std;

class Lexer{
  public:
    Lexer(const char* file_name);
    void Parse();
    bool SetDecl(string expression);
    bool Set(string expression);
    bool Ident(string expression);
    bool BasicSet(string expression);
    bool String(string expression);
    bool Char(string expression);
    bool KeywordDecl(string expression);
    bool TokenDecl(string expression);
    bool TokenExpr(string expression);
    bool TokenTerm(string expression);
    bool TokenFactor(string expression);
    bool Symbol(string expression);
    bool Number(string expression);
  private:
    ifstream file;
    const char* file_name;
    string or_operation, open_paranthesis, close_paranthesis, open_braces, close_braces, open_brackets, close_brackets, quote, apostrophe;
    string letter;
    string digit;
    string number;
    string string_;
    string character;
    string character0;
    string char_;
    string basicSet;
    string point;
    string set;
    string setDecl;
    string ident;
    static AFD* ident_AFD;
    static Tree* ident_tree;
    static vector<char> ident_lang;
    static AFD* number_AFD;
    static Tree* number_tree;
    static vector<char> number_lang;
};

#endif
