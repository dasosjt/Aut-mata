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
vertex* AFDX::init_vertex;
vertex* AFDX::final_vertex;

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

void AFDX::simulationAFDX(string exprsn){
  vector<vertex*> s0, S;
  vector<char> expression(exprsn.begin(), exprsn.end());
  expression.push_back('f');
  s0.push_back(init_vertex);
  S = eclosure(s0);
  char c = expression[0];
  expression.erase(expression.begin());
  while(c!='f'){
    S = move(S, c);
    c = expression[0];
    expression.erase(expression.begin());
  };
  bool final_state = false;
  cout << final_vertex->number_of << endl;
  for(unsigned int i = 0; i < S[0]->afdx_set.size(); i++){
    cout << S[0]->afdx_set[i] << endl;
    if (S[0]->afdx_set[i] == final_vertex->number_of){
      final_state = true;
    };
  }
  if(final_state){
    cout << "YES" << endl;
  } else {
    cout << "NO" << endl;
  }
}

AFDX* AFDX::subset_const(vertex* vertex_init, vector<char> L){
  stack<vector<vertex* > > Dstates;
  vector<vertex* > states_afdx;
  vector<vertex* > temp_init;
  temp_init.push_back(vertex_init);
  vector<vertex* > t0 = eclosure(temp_init);
  sort(t0.begin(), t0.end(), compare_by_number_of());
  init_vertex = subset_to_vertex(t0);
  init_vertex->number_of = get_new_state();
  states_afdx.push_back(init_vertex);
  Dstates.push(t0);
  cout << "Creating AFDX .... " << endl;
  while (!Dstates.empty()){
    vector<vertex* > T = Dstates.top();
    vertex* v_from = subset_to_vertex(T);
    Dstates.pop();
    for(unsigned int i = 0; i<L.size(); i++){
      vector<vertex* >  U = eclosure(move(T, L[i]));
      sort(U.begin(), U.end(), compare_by_number_of());
      vertex* v_to = subset_to_vertex(U);
      if(count_if(states_afdx.begin(), states_afdx.end(), compare_by_afdx_set(v_to->afdx_set)) == 0){
        cout << "New AFDX state found " << endl;
        v_to->number_of = get_new_state();
        states_afdx.push_back(v_to);
        Dstates.push(U);
      };
      vector<vertex* >::iterator it_v_from = find_if(states_afdx.begin(), states_afdx.end(), compare_by_afdx_set(v_from->afdx_set));
      vector<vertex* >::iterator it_v_to = find_if(states_afdx.begin(), states_afdx.end(), compare_by_afdx_set(v_to->afdx_set));
      int index_v_from = distance( states_afdx.begin(), it_v_from );
      int index_v_to = distance( states_afdx.begin(), it_v_to );
      cout << "State index from -> " << index_v_from << endl;
      cout << "State index to -> " << index_v_to << endl;
      cout << "with.. " << L[i] << endl;
      states_afdx[index_v_from]->vertex_to.push_back(make_pair(L[i], states_afdx[index_v_to]));
    };
  };
}

vertex* AFDX::subset_to_vertex(vector<vertex* > v){
  vertex* new_state = new vertex;
  if(v.size() > 0 ){
    for(unsigned int k = 0; k<v.size(); k++){
      new_state->afdx_set.push_back(v[k]->number_of);
    }
  } else {
    new_state->afdx_set.push_back(0);
  }
  cout << "{ ";
  copy(new_state->afdx_set.begin(), new_state->afdx_set.end(), ostream_iterator<int>(cout, " "));
  cout << "}"<< endl;
  return new_state;
}

vector<vertex* > AFDX::eclosure(vector<vertex* > v){
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
      final_vertex = t;
    };
  }
  result_v.insert(result_v.end(), v.begin(), v.end());
  return result_v;
}

vector<vertex* > AFDX::move(vector<vertex* > v, char c){
  vector<vertex* > result_v;
  //cout << "MOVE WITH "<< c << endl;
  //cout << "Cantidad de Estados actuales "<< v.size() << endl;
  if(!v.empty()){
    for (unsigned int i = 0; i<=v.size()-1; i++){
      //cout <<"   Estado en revision: "<< v[i]->number_of << endl;
      if(!v[i]->vertex_to.empty()){
        for(unsigned int j = 0; j<=v[i]->vertex_to.size()-1; j++){
          if(v[i]->vertex_to[j].first == c){
            result_v.push_back(v[i]->vertex_to[j].second);
          };
          //cout << "     Se mueve con '"<<v[i]->vertex_to[j].first << "' hacia estado "
          //     << v[i]->vertex_to[j].second->number_of << endl;
        };
      } else {
        //cout << "     No tiene movimiento, es estado final"<< endl;
        final_vertex = v[i];
      };
    };
  }
  return result_v;
}

vertex* AFDX::get_vertex_init(){
  return init_vertex;
}
