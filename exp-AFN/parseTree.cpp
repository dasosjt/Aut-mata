//g++ parseTree.cpp -o testBoost -Lc:\Boostbuild\lib -Ic:\Boostbuild\include\boost-1_6

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

boost::property_tree::ptree parseToTree(std::vector<char> tokens){
  boost::property_tree::ptree child;
  child.put("1", tokens[0]);
  child.put("2", tokens[1]);
  return child;
}

int main(){

  boost::property_tree::ptree tree;
  tree.put("test_name", "test_value");
  /*boost::property_tree::ptree child;

  for(int i = 0; i < 32; ++i){
    std::stringstream ss;
    ss << "value" << i;
    child.put(ss.str(), i);
  }

  tree.add_child("some values", child);
  tree.add("a.path.to.float.value", 3.14f);
  tree.add("b.path.to.float.value", 3.14f);*/

  std::string str("Sp");
  std::vector<char> tokens(str.begin(), str.end());
  for(int i = 0; i < tokens.size(); ++i){
    std::cout << tokens[i];
  }

  tree.add_child("some values", parseToTree(tokens));
  boost::property_tree::write_json("./output.json", tree);

  return 0;
}
