#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>


using namespace std;

AFDX::AFDX(){
  result = NULL;
}

void AFDX::createAFDX(vertex* vertex_init, vector<char> L){
  result = new AFDX;
  result = subset_const(vertex_init, L);
}

AFDX* AFDX::subset_const(vertex* vertex_init, vector<char> L){
  stack<vector<vertex* > > Dstates;
  vector<vector<vertex* > > states_afdx;
  vector<vertex* > temp_init;
  temp_init.push_back(vertex_init);
  states_afdx.push_back(eclosure(temp_init));
  Dstates.push(eclosure(temp_init));
  while (!Dstates.empty()){
    vector<vertex* > T = Dstates.top();
    Dstates.pop();
    for(unsigned int i = 0; i<=L.size()-1; i++){
      vector<vertex* >  U = eclosure(move(T, L[i]));
      if(count(states_afdx.begin(), states_afdx.end(), U)==0){
        states_afdx.push_back(U);
        Dstates.push(U);
      };
    };
  };
  cout << "Cantidad de estados del AFDX " << states_afdx.size() << endl;
  for(unsigned int k = 0; k <= states_afdx.size()-1; k++){
    vector<char> temp_substate;
    for (unsigned int n = 0; n<=states_afdx[k].size()-1; n++){
      temp_substate.push_back(states_afdx[k][n]->number_of);
    }
    string temp_state(temp_substate.begin(), temp_substate.end());
    cout << temp_state << endl;
  }
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
