#include "tree.h"
#include "afn.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>


using namespace std;

int AFN::state;
ofstream AFN::AFN_file;
ostringstream AFN::AFN_output_t;

AFN::AFN(){
  result = NULL;
}

int AFN::get_new_state(){
  state+=1;
  //cout <<"new state "<< state << endl;
  return state;
}

void AFN::createAFN(node* root, vector<char> L){
  this->L = L;
  result = new AFN;
  result = visitAST(root);
}

vertex* AFN::get_vertex_init_result(){
  return result->init_vertex;
}

void AFN::simulationAFN(string exprsn){
  vector<vertex*> s0, S;
  vector<char> expression(exprsn.begin(), exprsn.end());
  expression.push_back('f');
  s0.push_back(result->init_vertex);
  S = eclosure(s0);
  char c = expression[0];
  expression.erase(expression.begin());
  while(c!='f'){
    S = eclosure(move(S, c));
    c = expression[0];
    expression.erase(expression.begin());
  };
  bool final_state = false;
  if(S.size() != 0){
    for(unsigned int i = 0; i< S.size(); i++){
      if (S[i]->vertex_to.size() == 0){
        final_state = true;
      };
    };
  }
  if(final_state){
    cout << "YES" << endl;
  } else {
    cout << "NO" << endl;
  }
  AFN_file.open("AFN.txt", ios::out);
  if (AFN_file.is_open()) {
    AFN_file << "ESTADOS = ";
    AFN_file << states_to_text() << endl;
    AFN_file << "SIMBOLOS = ";
    AFN_file << symbols_to_text() << endl;
    AFN_file << "INICIO = ";
    AFN_file << init_to_text() << endl;
    AFN_file << "ACEPTACION = ";
    AFN_file << final_to_text() << endl;
    AFN_file << "TRANSICION = ";
    AFN_file << AFN_output_t.str() << endl;
  }
  AFN_file.close();
}

vector<vertex* > AFN::eclosure(vector<vertex* > v){
  vector< vertex* > result_v;
  stack< vertex*, vector< vertex* > > states (v);
  vertex* t;
  //cout << "E-CLOSURE "<< endl;
  //cout << "Cantidad de Estados actuales "<< v.size() << endl;
  while(!states.empty()){
    t = states.top();
    states.pop();
    //cout <<"   Estado en revision: "<< t->number_of << endl;
    if(!t->vertex_to.empty()){
      for(unsigned int j = 0; j<=t->vertex_to.size()-1; j++){
        if(t->vertex_to[j].first == 'e'){
          if(count(result_v.begin(), result_v.end(), t->vertex_to[j].second)==0){
              result_v.push_back(t->vertex_to[j].second);
              states.push(t->vertex_to[j].second);
          }
        };
      //  cout << "     Se mueve con '"<<t->vertex_to[j].first << "' hacia estado "
      //       << t->vertex_to[j].second->number_of << endl;
      };
    } else {
      //cout << "     No tiene movimiento, es estado final"<< endl;
    };
  }
  result_v.insert(result_v.end(), v.begin(), v.end());
  return result_v;
}

vector<vertex* > AFN::move(vector<vertex* > v, char c){
  vector<vertex* > result_v;
  //cout << "MOVE WITH "<< c << endl;
  //cout << "Cantidad de Estados actuales "<< v.size() << endl;
  if(!v.empty()){
    for (unsigned int i = 0; i<=v.size()-1; i++){
    //  cout <<"   Estado en revision: "<< v[i]->number_of << endl;
      if(!v[i]->vertex_to.empty()){
        for(unsigned int j = 0; j<=v[i]->vertex_to.size()-1; j++){
          if(v[i]->vertex_to[j].first == c){
            result_v.push_back(v[i]->vertex_to[j].second);
          };
      //    cout << "     Se mueve con '"<<v[i]->vertex_to[j].first << "' hacia estado "
        //       << v[i]->vertex_to[j].second->number_of << endl;
        };
      } else {
        //cout << "     No tiene movimiento, es estado final"<< endl;
      };
    };
  }
  return result_v;
}

vertex* AFN::get_vertex_init(){
  return init_vertex;
}

vertex* AFN::get_vertex_final(){
  return final_vertex;
}

void AFN::set_vertex_final_to(char e, vertex* v){
  final_vertex->vertex_to.push_back(make_pair(e, v));
}

AFN* AFN::visitAST(node* current){
  if(current != NULL){
    char key_value = current->key_value;
    //cout << current->key_value << endl;
    if(key_value == '|'){
      return orAFN(visitAST(current->right), visitAST(current->left));
    } else if( key_value == '^' ) {
      return concAFN(visitAST(current->right), visitAST(current->left));
    } else if ( key_value == '*'){
      return kleeneAFN(visitAST(current->right));
    } else {
      return base(current);
    }
  }
}

void AFN::tran_to_text(int from, int to, char a){
  ostringstream temp;
  temp << "(" << from << ", " << a << ", " << to << ")";
  AFN_output_t << temp.str();
}

string AFN::states_to_text(){
  ostringstream temp;
  temp << "{";
  for(unsigned int i = 1; i<=state; i++){
    temp << i << ", ";
  }
  temp << "}";
  return temp.str();
}

string AFN::symbols_to_text(){
  ostringstream temp;
  temp << "{";
  copy(this->L.begin(), this->L.end(), ostream_iterator<char>(temp, ", "));
  temp << "}";
  return temp.str();
}

string AFN::final_to_text(){
  ostringstream temp;
  temp << "{" << result->final_vertex->number_of << "}";
  return temp.str();
}

string AFN::init_to_text(){
  ostringstream temp;
  temp << "{" << result->init_vertex->number_of << "}";
  return temp.str();
}

AFN* AFN::base(node* current){
  //cout << "Creando AFN para " << current->key_value << endl;
  AFN* result = new AFN;
  vertex* v1 = new vertex;
  vertex* v2 = new vertex;
  v1->number_of = get_new_state();
  v2->number_of = get_new_state();
  v1->vertex_to.push_back(make_pair(current->key_value, v2));
  result->init_vertex = v1;
  result->final_vertex = v2;
  tran_to_text(v1->number_of, v2->number_of, current->key_value);
  return result;
}

AFN* AFN::orAFN(AFN* a, AFN* b){
  //cout << "Creando AFN para |" << endl;
  AFN* result = new AFN;
  vertex* v1 = new vertex;
  vertex* v2 = new vertex;
  v1->number_of = get_new_state();
  v2->number_of = get_new_state();
  v1->vertex_to.push_back(make_pair('e', a->get_vertex_init()));
  v1->vertex_to.push_back(make_pair('e', b->get_vertex_init()));
  a->set_vertex_final_to('e', v2);
  b->set_vertex_final_to('e', v2);
  tran_to_text(v1->number_of, a->get_vertex_init()->number_of, 'e');
  tran_to_text(v1->number_of, b->get_vertex_init()->number_of, 'e');
  tran_to_text(a->get_vertex_final()->number_of, v2->number_of, 'e');
  tran_to_text(b->get_vertex_final()->number_of, v2->number_of, 'e');
  result->init_vertex = v1;
  result->final_vertex = v2;
  return result;
}

AFN* AFN::kleeneAFN(AFN* a){
  //cout << "Creando AFN para *" << endl;
  AFN* result = new AFN;
  vertex* v1 = new vertex;
  vertex* v2 = new vertex;
  v1->number_of = get_new_state();
  v2->number_of = get_new_state();
  a->set_vertex_final_to('e', a->get_vertex_init());
  a->set_vertex_final_to('e', v2);
  v1->vertex_to.push_back(make_pair('e', v2));
  v1->vertex_to.push_back(make_pair('e', a->get_vertex_init()));
  result->init_vertex = v1;
  result->final_vertex = v2;
  tran_to_text(a->get_vertex_final()->number_of, a->get_vertex_init()->number_of, 'e');
  tran_to_text(a->get_vertex_final()->number_of, v2->number_of, 'e');
  tran_to_text(v1->number_of, v2->number_of, 'e');
  tran_to_text(v1->number_of, a->get_vertex_init()->number_of, 'e');
  return result;
}

AFN* AFN::concAFN(AFN* a, AFN* b){
  //cout << "Creando AFN para ^" << endl;
  AFN* result = new AFN;
  b->set_vertex_final_to('e', a->get_vertex_init());
  result->init_vertex = b->get_vertex_init();
  result->final_vertex = a->get_vertex_final();
  tran_to_text(b->get_vertex_init()->number_of, a->get_vertex_final()->number_of, 'e');
  return result;
}
