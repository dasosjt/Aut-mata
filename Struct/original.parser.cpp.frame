#include <string>
#include <vector>
#include <algorithm>

#include "parser.h"

using namespace std;

Parser::Parser(){

}

void Parser::setNextTokens(vector< string > nextTokens0){
  this->nextTokens = nextTokens0;
}

void Parser::consume(){
  nextTokens.erase(begin(nextTokens));
}

string Parser::la(){
  if(!nextTokens.empty()){
    return nextTokens[0];
  }else{
    return "";
  }
}

//-->methods
