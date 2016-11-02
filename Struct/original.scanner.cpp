/*#include "scanner.h"
#include <fstream>
#include <vector>

using namespace std;

Scanner::Scanner(){
init_vertex = new vertex;
//-->size
  AFN_lineal.reserve(n);
  for(vertex* v : AFN_lineal){
    v = new vertex;
  }
//-->init
//-->transitions
//-->finals
}

void Scanner::Parse(){
  string line;
  ifstream file("texto.txt");
  size_t pos = 0;
  string token;
  while (std::getline(file, line)){
    while((pos = line.find(' ')) != string::npos){
      token = line.substr(0, pos);
      cout << token << endl;
      compiler_AFN->simulationAFN(token);
      line.erase(0, pos+1);
    }
  }
}
*/
