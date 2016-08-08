#include <iostream>
#include <string>

int main(){
  /*std::cout << "Have " << argc << " arguments:" << std::endl;
  for(int i = 0; i < argc; i++){
    std::cout << argv[i] << std::endl;
  }*/
  std::string exprsn;
  std::cout << "Ingregsar expresion regular ";
  getline (std::cin, exprsn);
  std::cout << exprsn << "\n";
}
