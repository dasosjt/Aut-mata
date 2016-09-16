#ifndef AFD_H
#define AFD_H


using namespace std;

class AFD{
  public:
    AFD();
    void set_root(node* root);
    int get_new_state();
    void set_id_number(node* root);
    bool set_voidable(node* root);
    vector<int> set_firstpos(node* root);
    vector<int> set_lastpos(node* root);
    node* search_node(node* root, int id_number);
    void set_nextpos(node* root);

  private:
    node* root;
    static int state;

};

#endif
