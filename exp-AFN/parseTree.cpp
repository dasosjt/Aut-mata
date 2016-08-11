//g++ parseTree.cpp -o testBoost -Lc:\Boostbuild\lib -Ic:\Boostbuild\include\boost-1_6

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//Funcion recursiva para armar el arbol
boost::property_tree::ptree parseToTree(std::vector<char> tokens){
  //Un nuevo child de un arbol
  boost::property_tree::ptree child;
  //Si el vector es mayor a 1, entonces separamos el arbol.
  if(tokens.size() > 1){
    std::vector<char> subtokens(tokens.begin()+1, tokens.end());
    child.put("0", tokens[0]);
    child.add_child("1", parseToTree(subtokens));
  } else {
    child.put("1", tokens[0]);
  }
  return child;
}

int main(){
  //Creamos la raiz principal de arbol
  boost::property_tree::ptree tree;
  tree.put("test_name", "test_value");

  std::string str("Spjdst");
  std::vector<char> tokens(str.begin(), str.end());

  for(int i = 0; i < tokens.size(); ++i){
    std::cout << tokens[i];
  }

  //invertimos el vector
  std::reverse(tokens.begin(),tokens.end());

  tree.add_child("some values", parseToTree(tokens));
  boost::property_tree::write_json("./output.json", tree);

  return 0;
}
