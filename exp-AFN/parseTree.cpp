//g++ parseTree.cpp -o testBoost -Lc:\Boostbuild\lib -Ic:\Boostbuild\include\boost-1_6

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//Funcion recursiva para armar el arbol
boost::property_tree::ptree parseToTree(std::vector<char> tokens, std::string c){
  //Un nuevo child de un arbol
  boost::property_tree::ptree child;

  /*if(tokens[0] == '('){
    std::vector<char> subtokens(tokens.begin()+1, tokens.end());
    child.add_child("()", parseToTree(subtokens, "2"));
  }*/

  return child;
}

int main(){
  //Creamos la raiz principal de arbol
  boost::property_tree::ptree tree;

  std::string str("(a)");
  std::vector<char> tokens(str.begin(), str.end());

  //invertimos el vector
  /*std::reverse(tokens.begin(),tokens.end());*/
  /*it = std::find_end (tokens.begin(), tokens.end());
  std::cout << it << std::endl;*/

  /*tree.add_child("parseToTree", parseToTree(tokens, "1"));
  boost::property_tree::write_json("./output.json", tree);*/
  std::string::size_type last = str.find_last_of(")");
  std::cout << last;
  return 0;
}
