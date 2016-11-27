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
    void setFirst();
    vector< string > firstOf(vertex_asg* current);
    void setFollow();
    void followOfR2(vertex_asg* current);
    void followOfR3(vertex_asg* current);

  private:
    vertex_asg* root;
    vertex_asg* current_production;
    vector< vertex_asg* > productions_root;
    vector< vector< string > > productions_first;
    stack<char> pbb_signs;
    stack<string> current_pbb_signs;
    int count_or;
};


#endif
