#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include "lexer.h"
#include <string>
#include <iostream>
#include <iterator>

using namespace std;

int main(int argc, char *argv[]){
  Lexer* lex = new Lexer("cocol.txt");
  lex->Parse();
  lex->symbol_to_AFN();
}
