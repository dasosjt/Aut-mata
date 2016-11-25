#include <string>
#include <vector>
#include <iostream>
#include "asg.h"


ASG::ASG(){
  vertex_asg* root = new vertex_asg;
  this->root = root;
}

void ASG::create_ProductionsRoot(vector< string > productions){
  for(string p : productions){
    vertex_asg* newp = new vertex_asg;
    newp->id = p;
    newp->production_root = true;
    productions_root.push_back(newp);
  }
}
