#ifndef AFN_H
#define AFN_H


#include <string>
#include <utility>
#include <vector>
#include <stack>

using namespace std;

struct vertex{
  int number_of;
  vector<int> afdx_set;
  vector< pair<char, vertex*> > vertex_to;
};

class AFN{
  public:
    AFN();
    void createAFN(node* root);
    AFN* visitAST(node* root);
    AFN* base(node* current);
    AFN* orAFN(AFN* a, AFN* b);
    AFN* kleeneAFN(AFN* a);
    AFN* concAFN(AFN* a, AFN* b);
    int get_new_state();
    vertex* get_vertex_init_result();
    vertex* get_vertex_init();
    vertex* get_vertex_final();
    void set_vertex_final_to(char e, vertex* v);
    void simulationAFN(string exprsn);
    vector<vertex*> eclosure(vector<vertex*> v);
    vector<vertex*> move(vector<vertex* > v, char c);

  private:
    vertex* init_vertex;
    vertex* final_vertex;
    AFN* result;
    stack<vertex* > actual_states;
    stack<vertex* > new_states;
    static int state;
};

#endif
