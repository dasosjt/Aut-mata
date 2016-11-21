#include <string>
#include <vector>
#include "asg.h"

using namespace std;

ASG::ASG(){
  vertex_asg* root = new vertex_asg;
  this->root = root;
};
