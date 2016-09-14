#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <stack>
#include <string>
#include <algorithm>


using namespace std;

int AFDX::state;

AFDX::AFDX(){
  result = NULL;
}

int AFDX::get_new_state(){
  state+=1;
  cout <<"new state "<< state << endl;
  return state;
}

void AFDX::createAFDX(vertex* vertex_init, vector<char> L){
  result = new AFDX;
  result = subset_const(vertex_init, L);
}

AFDX* AFDX::subset_const(vertex* vertex_init, vector<char> L){
  stack<vector<vertex* > > Dstates;
  vector<vertex* > states_afdx;
  vector<vertex* > temp_init;
  temp_init.push_back(vertex_init);
  vector<vertex* > t0 = eclosure(temp_init);
  sort(t0.begin(), t0.end(), compare_by_number_of());
  states_afdx.push_back(subset_to_vertex(t0));
  Dstates.push(t0);
  while (!Dstates.empty()){
    vector<vertex* > T = Dstates.top();
    Dstates.pop();
    for(unsigned int i = 0; i<L.size(); i++){
      vector<vertex* >  U = eclosure(move(T, L[i]));
      if (U.size()>0){
        sort(U.begin(), U.end(), compare_by_number_of());
        vertex* v_temp = subset_to_vertex(U);
        if(count_if(states_afdx.begin(), states_afdx.end(), compare_by_afdx_set(v_temp->afdx_set)) == 0){
          cout << "Nuevo estado AFDX encontrado " << endl;
          states_afdx.push_back(v_temp);
          Dstates.push(U);
        } else {
          vector<vertex* >::iterator it = find_if(states_afdx.begin(), states_afdx.end(), compare_by_afdx_set(v_temp->afdx_set));
          int index = distance( states_afdx.begin(), it );
          cout << "State index : " << index << endl;
        };
      };
    };
  };
}

vertex* AFDX::subset_to_vertex(vector<vertex* > v){
  vertex* new_state = new vertex;
  if(v.size() > 0 ){
    for(unsigned int k = 0; k<v.size(); k++){
      cout << v[k]->number_of << endl;
      new_state->afdx_set.push_back(v[k]->number_of);
    }
  }
  return new_state;
}

vector<vertex* > AFDX::eclosure(vector<vertex* > v){
  vector< vertex* > result_v;
  stack< vertex*, vector< vertex* > > states (v);
  vertex* t;
  cout << "E-CLOSURE "<< endl;
  cout << "Cantidad de Estados actuales "<< v.size() << endl;
  while(!states.empty()){
    t = states.top();
    states.pop();
    cout <<"   Estado en revision: "<< t->number_of << endl;
    if(!t->vertex_to.empty()){
      for(unsigned int j = 0; j<=t->vertex_to.size()-1; j++){
        if(t->vertex_to[j].first == 'e'){
          if(count(result_v.begin(), result_v.end(), t->vertex_to[j].second)==0){
              result_v.push_back(t->vertex_to[j].second);
              states.push(t->vertex_to[j].second);
          }
        };
        cout << "     Se mueve con '"<<t->vertex_to[j].first << "' hacia estado "
             << t->vertex_to[j].second->number_of << endl;
      };
    } else {
      cout << "     No tiene movimiento, es estado final"<< endl;
    };
  }
  result_v.insert(result_v.end(), v.begin(), v.end());
  return result_v;
}

vector<vertex* > AFDX::move(vector<vertex* > v, char c){
  vector<vertex* > result_v;
  cout << "MOVE WITH "<< c << endl;
  cout << "Cantidad de Estados actuales "<< v.size() << endl;
  if(!v.empty()){
    for (unsigned int i = 0; i<=v.size()-1; i++){
      cout <<"   Estado en revision: "<< v[i]->number_of << endl;
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

vertex* AFDX::get_vertex_init(){
  return init_vertex;
}

vertex* AFDX::get_vertex_final(){
  return final_vertex;
}
