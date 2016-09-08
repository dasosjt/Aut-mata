#ifndef AFDX_H
#define AFDX_H


#include <string>
#include <vector>
#include <stack>

using namespace std;

struct compare_vertex_by_number_of {
    bool operator()(vertex const * a, vertex const * b) {
        return a->number_of < b->number_of;
    }
};

class AFDX{
  public:
    AFDX();
    void createAFDX(vertex* v1, vector<char> L);
    AFDX* subset_const(vertex* v1, vector<char> L);
    vertex* get_vertex_init();
    vertex* get_vertex_final();
    vector<vertex*> eclosure(vector<vertex*> v);
    vector<vertex*> move(vector<vertex* > v, char c);
    vertex* convertToVertex(vector< vertex* > v);


  private:
    vertex* init_vertex;
    vertex* final_vertex;
    AFDX* result;
    stack<vertex* > actual_states;
    stack<vertex* > new_states;
};

#endif
