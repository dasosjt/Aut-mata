//g++ main.cpp -o testBoost -Lc:\Boostbuild\lib -Ic:\Boostbuild\include\boost-1_6

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

int main(){

  boost::property_tree::ptree tree;
  tree.put("test_name", "test_value");

  boost::property_tree::ptree child;

  for(int i = 0; i < 32; ++i){
    std::stringstream ss;
    ss << "value" << i;
    child.put(ss.str(), i);
  }

  tree.add_child("some values", child);

  boost::property_tree::write_json("./output.json", tree);

  return 0;
}
