#ifndef AFD_H
#define AFD_H


using namespace std;

class AFD{
  public:
    AFD();
    void set_root(node* root);
    int get_new_id_number();
    int get_new_state();
    void set_id_number(node* root);
    bool set_voidable(node* root);
    vector<int> set_firstpos(node* root);
    vector<int> set_lastpos(node* root);
    node* search_node(node* root, int id_number);
    void set_nextpos(node* root);
    void createAFD(node* root, vector<char> L);
    void minAFD();
    void simulationAFD(string exprsn);
    vector<vertex* > move(vector<vertex* > v, char c);
    void tran_to_text(int from, int to, char a);
    string states_to_text();
    string symbols_to_text();
    string final_to_text();
    string init_to_text();

  private:
    node* root;
    static vertex* init_vertex;
    static vertex* final_vertex;
    static int new_id_number;
    static int state;
    vector<vertex* > states_afd;
    vector<char> L;
    static ofstream AFD_file;
    static ostringstream AFD_output_t;

};

#endif
