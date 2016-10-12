#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include "lexer.h"
#include <string>
#include <iostream>
#include <iterator>
#include <ctype.h>

using namespace std;

int main(int argc, char *argv[]){
  Lexer* lex = new Lexer("cocol.txt");
  //lex->Parse();
  if(lex->SetDecl("aaaa = bbbb.")){
    cout << "SetDecl.. check" << endl;
  };
  if(lex->KeywordDecl("aaaa = bbbb .")){
    cout << "KeywordDecl.. check" << endl;
  };
  if(lex->TokenDecl("aaaa = {aaa} | [abb].")){
    cout << "TokenDecl.. check" << endl;
  };
  /*string letter = "a..z";
  string digit = "0|1|2|3|4|5|6|7|8|9";
  string hexdigit = digit+"A..F";*/
  /*vector<char> L;
  Tree* tree = new Tree();
  string exprsn;
  cout << "Enter Regular Expression " << endl;
  getline(cin, exprsn);
  tree->parse(exprsn);
  cout << "AST " << endl;
  tree->display();
  L = tree->getL();
  cout << "AFN START" << endl;
  AFN* afn = new AFN();
  afn->createAFN(tree->getRoot(), L);
  cout << "AFN READY FOR TEST" << endl;
  cout << "Enter Expression " << endl;
  cin >> exprsn ;
  afn->simulationAFN(exprsn);
  AFDX* afdx = new AFDX();
  cout << "AFN TO AFD "<< endl;
  afdx->createAFDX(afn->get_vertex_init_result(), L);
  cout << "AFDX READY FOR TEST" << endl;
  cout << "Enter Expression " << endl;
  cin >> exprsn ;
  afdx->simulationAFDX(exprsn);
  afdx->minAFD();
  cout << "AST TO AFD "<< endl;
  AFD* afd = new AFD();
  afd->createAFD(tree->getRoot(), L);
  cout << "AFD READY FOR TEST" << endl;
  cout << "Enter Expression " << endl;
  cin >> exprsn ;
  afd->simulationAFD(exprsn);
  afd->minAFD();*/
}
