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
    if(line.find(' ') == string::npos){
      cout << line << endl;
      compiler_AFN->simulationAFN(line);
    }else{
      while((pos = line.find(' ')) != string::npos){
        token = line.substr(0, pos);
        cout << token << endl;
        compiler_AFN->simulationAFN(token);
        line.erase(0, pos+1);
      }
    }
  }
}*/
