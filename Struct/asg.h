#ifndef ASG_H
#define ASG_H

using namespace std;

struct vertex_asg{
  string id;
  vector< string > first;
  vector< string > follow;
  bool production_root;
  vector< pair<string, vertex_asg*> > vertex_to;
};

class ASG{
  public:
    ASG();
  private:
    vertex_asg* root;
    vector< vertex_asg* > productions_root;
};


#endif
