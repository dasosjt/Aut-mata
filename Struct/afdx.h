#ifndef AFDX_H
#define AFDX_H


#include <string>
#include <vector>
#include <stack>

using namespace std;

struct compare_by_number_of {
    bool operator()(vertex const * a, vertex const * b) {
        return a->number_of < b->number_of;
    }
};

struct compare_by_afdx_set{
    explicit compare_by_afdx_set(const vector<int>& vint) : afdx_set(vint) {}
    bool operator () (const vertex* v) const{
        return v->afdx_set == afdx_set;
    }
    vector<int> afdx_set;
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
    vertex* subset_to_vertex(vector<vertex* > v);
    int get_new_state();
    void simulationAFDX(string exprsn);
    void tran_to_text(int from, int to, char a);
    string states_to_text();
    string symbols_to_text();
    string final_to_text();
    string init_to_text();
    void minAFD();

  private:
    AFDX* result;
    vector<vertex* > states_afdx;
    stack<vertex* > actual_states;
    stack<vertex* > new_states;
    static int state;
    static vertex* init_vertex;
    static vertex* final_vertex;
    vector<char> L;
    static ofstream AFDX_file;
    static ostringstream AFDX_output_t;
};

#endif
