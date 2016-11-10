/*#include "tree.h"
#include "afn.h"
#include "scanner.h"
#include "string"
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

using namespace std;

Scanner::Scanner(){
  init_vertex = new vertex;
  unsigned int c = 0;
//-->size
  AFN_lineal.reserve(n);
  while(c<n){
    vertex* v = new vertex;
    AFN_lineal[c] = v;
    AFN_lineal[c]->number_of = c;
    c++;
  }
//-->init
//-->transitions
//-->finals
   compiler_AFN = new AFN();
   compiler_AFN->set_init_state(init_vertex);
}

void Scanner::Parse(){
  string line;
  ifstream file("texto.txt");
  size_t pos = 0;
  string token;
  while (std::getline(file, line)){
    cout << "Start of line: "<< endl << line << endl << endl;
    bool got_token = true;
    while(!line.empty()){
      cout << line << endl;
      string c = line.substr(0,1);
      line.erase(0,1);
      got_token = compiler_AFN->simulationAFN(c, got_token);
    }
  }
}*/
