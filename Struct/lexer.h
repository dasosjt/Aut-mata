#ifndef LEXER_H
#define LEXER_H

#include "lexer.h"
#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stack>

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
    void cout_symbol_table();
    void cout_type_table();
    void add_symbol_table(string symbol_to_append);
    void add_type_table(string ident);
    string tokenToHash(string expression);
    string signToSpecialChar(string string);
    void symbol_to_AFN();
    void printExpression(string exprsn);
    void cleanMinusSigns();

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
    string constr;
    string endof_line;
    string whitespace;
    string anything;
    static AFD* ident_AFD;
    static Tree* ident_tree;
    static vector<char> ident_lang;
    static AFD* number_AFD;
    static Tree* number_tree;
    static vector<char> number_lang;
    static AFD* constr_AFD;
    static Tree* constr_tree;
    static vector<char> constr_lang;
    static unordered_map<string,string> symbol_table;
    static unordered_map<string,string> type_table;
    stack<char> pbb_signs;
    stack<string> current_pbb_signs;
    int count_or;
    string current_ident;
    string typeDecl;
    string file_contents;
    vector<AFD* > primitives_AFD;
    string cocol_name;
};

#endif
