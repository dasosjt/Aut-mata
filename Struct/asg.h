#ifndef ASG_H
#define ASG_H

using namespace std;

struct vertex_asg{
  string id;
  vector< string > first;
  vector< string > follow;
  bool production_root;
  vector< vertex_asg* > vertex_to;
};

class ASG{
  public:
    ASG();
    void create_ProductionsRoot(vector< string > productions);
    vertex_asg* vertex_byExpression(string expression);
    vertex_asg* vertex_byTerm(string expression);
    vertex_asg* vertex_byFactor(string expression);
    bool create_ASG(string production_name, string expression);
  private:
    vertex_asg* root;
    vector< vertex_asg* > productions_root;
    stack<char> pbb_signs;
    stack<string> current_pbb_signs;
    int count_or;
};


#endif
