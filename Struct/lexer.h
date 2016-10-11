#ifndef LEXER_H
#define LEXER_H

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
  private:
    ifstream file;
    const char* file_name;
    string letter;
    string digit;
};

#endif
