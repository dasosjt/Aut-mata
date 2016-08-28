#include "tree.h"
#include "afn.h"
#include <iostream>
#include <string>


using namespace std;

AFN::AFN(){

}

void AFN::createAFN(node* root){
  result = new AFN;
  result = visitAST(root);
}

void AFN::simulationAFN(string exprsn){
  vector<vertex*> s0;
  s0.push_back(result->init_vertex);
  vector<vertex*> S;
  S = eclosure(s0);
  S = eclosure(S);
  S = eclosure(move(S, 'a'));

}

vector<vertex* > AFN::eclosure(vector<vertex* > v){
  vector<vertex* > result_v;
  cout << "E-CLOSURE "<< endl;
  cout << "Estados actuales "<< v.size() << endl;
  if(!v.empty()){
    for (unsigned int i = 0; i<=v.size()-1; i++){
      cout <<"   Estado en revision "<< v[i]->number_of << endl;
      if(!v[i]->vertex_to.empty()){
        for(unsigned int j = 0; j<=v[i]->vertex_to.size()-1; j++){
          if(v[i]->vertex_to[j].first == 'e'){
            result_v.push_back(v[i]->vertex_to[j].second);
          };
          cout << "     Se mueve con '"<<v[i]->vertex_to[j].first << "' hacia estado "
               << v[i]->vertex_to[j].second->number_of << endl;
        };
      } else {
        cout << "     No tiene movimiento, es estado final"<< endl;
      };
    };
  }
  return result_v;
}

vector<vertex* > AFN::move(vector<vertex* > v, char c){
  vector<vertex* > result_v;
  cout << "MOVE WITH "<< c << endl;
  cout << "Estados actuales "<< v.size() << endl;
  if(!v.empty()){
    for (unsigned int i = 0; i<=v.size()-1; i++){
      cout <<"   Estado en revision "<< v[i]->number_of << endl;
      if(!v[i]->vertex_to.empty()){
        for(unsigned int j = 0; j<=v[i]->vertex_to.size()-1; j++){
          if(v[i]->vertex_to[j].first == c){
            result_v.push_back(v[i]->vertex_to[j].second);
          };
          cout << "     Se mueve con '"<<v[i]->vertex_to[j].first << "' hacia estado "
               << v[i]->vertex_to[j].second->number_of << endl;
        };
      } else {
        cout << "     No tiene movimiento, es estado final"<< endl;
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

AFN* AFN::base(node* current){
  cout << "Creando AFN para " << current->key_value << endl;
  AFN* result = new AFN;
  vertex* v1 = new vertex;
  vertex* v2 = new vertex;
  v1->number_of = '2';
  v2->number_of = '3';
  v1->vertex_to.push_back(make_pair(current->key_value, v2));
  result->init_vertex = v1;
  result->final_vertex = v2;
  return result;
}

AFN* AFN::orAFN(AFN* a, AFN* b){
  cout << "Creando AFN para |" << endl;
  AFN* result = new AFN;
  vertex* v1 = new vertex;
  vertex* v2 = new vertex;
  v1->number_of = '1';
  v2->number_of = '6';
  v1->vertex_to.push_back(make_pair('e', a->get_vertex_init()));
  v1->vertex_to.push_back(make_pair('e', b->get_vertex_init()));
  a->set_vertex_final_to('e', v2);
  b->set_vertex_final_to('e', v2);
  result->init_vertex = v1;
  result->final_vertex = v2;
  return result;
}

AFN* AFN::kleeneAFN(AFN* a){
  cout << "Creando AFN para *" << endl;
  AFN* result = new AFN;
  vertex* v1 = new vertex;
  vertex* v2 = new vertex;
  v1->number_of = '0';
  v2->number_of = '7';
  a->set_vertex_final_to('e', a->get_vertex_init());
  a->set_vertex_final_to('e', v2);
  v1->vertex_to.push_back(make_pair('e', v2));
  v1->vertex_to.push_back(make_pair('e', a->get_vertex_init()));
  result->init_vertex = v1;
  result->final_vertex = v2;
  return result;
}

AFN* AFN::concAFN(AFN* a, AFN* b){
  cout << "Creando AFN para ^" << endl;
  AFN* result = new AFN;
  a->set_vertex_final_to('e', b->get_vertex_init());
  result->init_vertex = a->get_vertex_init();
  result->final_vertex = b->get_vertex_final();
  return result;
}
