#include "tree.h"
#include "afn.h"
#include "afd.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>


using namespace std;

int AFN::state;
fstream AFN::AFN_file;
ostringstream AFN::AFN_output_t;
ostringstream AFN::AFN_output_f;

AFN::AFN(){
  result = NULL;
}

void AFN::newfi_vertex(int state){
  this->state = state;
  this->init_vertex = new vertex;
  this->init_vertex->number_of = get_new_state();
  this->final_vertex = new vertex;
  this->final_vertex->number_of = get_new_state();
}
void AFN::set_init_state(vertex* init){
  this->init_vertex = new vertex;
  this->init_vertex = init;
}

void AFN::setL(vector<char> L){
  this->L = L;
}

int AFN::get_state(){
  return this->state;
}

/*void AFN::createAFN_AFD(vector<AFD* > list_AFD){
  this->init_vertex = new vertex;
  this->final_vertex = new vertex;
}*/

int AFN::get_new_state(){
  /*Agrego 1 al estado anterior y lo devuelvo*/
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
  /*Devuelvo es vertice/estado inicial*/
  return result->init_vertex;
}

void AFN::simulationAFN(string exprsn){
  /*Agrego un caracter que denota que es el final de leer caracteres, en este caso es el f*/
  vector<vertex*> s0, S;
  vector<char> expression(exprsn.begin(), exprsn.end());
  expression.push_back(char(254));
  s0.push_back(init_vertex);
  S = eclosure(s0);
  char c = expression[0];
  expression.erase(expression.begin());
  /*mientras no sea f... */
  while(c!=char(254)){
    /*Hago el move correspondiente*/
    S = eclosure(move(S, c));
    cout << c << endl;
    for(auto v : S){
      if(!v->token_id.empty()){
        cout << " TOKEN ID " << v->token_id << endl;
      }
      cout << " NUMBER OF " << v->number_of << endl;
    }
    c = expression[0];
    expression.erase(expression.begin());
  };
  bool final_state = false;
  /*Si el estado resultante no tiene el estado final, entonces respondo NO; pero si lo tiene entonces SI*/
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
}

void AFN::writeToFile(const char* file_name){
  string file_contents;
  string temp;
  ostringstream converter;
  AFN_file.open(file_name);
  if(AFN_file.is_open()){
    file_contents = {istreambuf_iterator<char>(AFN_file), istreambuf_iterator<char>()};
    AFN_file.close();
  }
  //cout << file_contents << endl;
  unsigned int size = file_contents.find("//-->size");
  converter << state;
  file_contents.insert(size+10, "   unsigned int n = " + converter.str() + ";\n");
  converter.str("");
  converter.clear();
  converter << state - 2;
  unsigned int init = file_contents.find("//-->init");
  file_contents.insert(init+10, "   init_vertex = AFN_lineal[" + converter.str() + "];\n");
  unsigned int transitions = file_contents.find("//-->transitions");
  file_contents.insert(transitions+17, AFN_output_t.str());
  unsigned int finals = file_contents.find("//-->finals");
  file_contents.insert(finals+12, AFN_output_f.str());
  //cout << file_contents << endl;

  ofstream write(file_name);
  write << file_contents;
  write.close();
}

void AFN::mapAFN(vertex* current_vertex){
  unsigned int vertex_from = current_vertex->number_of;
  if(current_vertex->token_id != ""){
      AFN_output_f << "   AFN_lineal[" << current_vertex->number_of-1 << "]->token_id = \"" << current_vertex->token_id << "\";" << endl;
  }
  traveled.push_back(vertex_from);
  for(auto trans : current_vertex->vertex_to){
    unsigned int vertex_to = trans.second->number_of;
    char letter = trans.first;
    if(count(traveled.begin(), traveled.end(), vertex_to) == 0){
      //cout << "Found new path " << vertex_from << " to "  << vertex_to << endl;
      mapAFN(trans.second);
      //cout << "Comeback to " << vertex_from << endl;
    }
    if(letter == char(238)){
      AFN_output_t << "   AFN_lineal[" << vertex_from-1 << "]->vertex_to.push_back(make_pair(char(" << 238 << "), AFN_lineal[" << vertex_to-1 << "]));" << endl ;
    }else{
      AFN_output_t << "   AFN_lineal[" << vertex_from-1 << "]->vertex_to.push_back(make_pair('" << trans.first << "', AFN_lineal[" << vertex_to-1 << "]));" << endl ;
    }
  }
  //cout << endl << endl << endl;
}

void AFN::coutTraveled(){
  copy(traveled.begin(), traveled.end(), ostream_iterator<int>(cout, ", "));
  cout << endl;
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
        if(t->vertex_to[j].first == char(238)){
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
  /*devuelvo el vertice inicial*/
  return init_vertex;
}

vertex* AFN::get_vertex_final(){
  /*devuelvo el vertice final*/
  return final_vertex;
}

void AFN::set_vertex_final_to(char e, vertex* v){
  /*agrego al  vertice final un vector y el caracter para hacer el movimiento*/
  final_vertex->vertex_to.push_back(make_pair(e, v));
}

AFN* AFN::visitAST(node* current){
  if(current != NULL){
    /*Visito el arbol para conforme lo viste forme el AFN*/
    char key_value = current->key_value;
    //cout << current->key_value << endl;
    if(key_value == char(179)){
      return orAFN(visitAST(current->right), visitAST(current->left));
    } else if( key_value == char(167) ) {
      return concAFN(visitAST(current->right), visitAST(current->left));
    } else if ( key_value == char(241)){
      return kleeneAFN(visitAST(current->right));
    } else {
      return base(current);
    }
  }
}

void AFN::tran_to_text(int from, int to, char a){
  ostringstream temp;
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  temp << "(" << from << ", " << a << ", " << to << ")";
  AFN_output_t << temp.str();
}

string AFN::states_to_text(){
  ostringstream temp;
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  temp << "{";
  for(unsigned int i = 1; i<=state; i++){
    temp << i << ", ";
  }
  temp << "}";
  return temp.str();
}

string AFN::symbols_to_text(){
  ostringstream temp;
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  temp << "{";
  copy(this->L.begin(), this->L.end(), ostream_iterator<char>(temp, ", "));
  temp << "}";
  return temp.str();
}

string AFN::final_to_text(){
  ostringstream temp;
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  temp << "{" << result->final_vertex->number_of << "}";
  return temp.str();
}

string AFN::init_to_text(){
  ostringstream temp;
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  temp << "{" << result->init_vertex->number_of << "}";
  return temp.str();
}

AFN* AFN::base(node* current){
  //cout << "Creando AFN para " << current->key_value << endl;
  /*Creo un AFN base con un caracter*/
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
  /*Uno a y b con una transicion or, agrengado dos vertices, final e inicial, para simular el OR*/
  AFN* result = new AFN;
  vertex* v1 = new vertex;
  vertex* v2 = new vertex;
  v1->number_of = get_new_state();
  v2->number_of = get_new_state();
  v1->vertex_to.push_back(make_pair(char(238), a->get_vertex_init()));
  v1->vertex_to.push_back(make_pair(char(238), b->get_vertex_init()));
  a->set_vertex_final_to(char(238), v2);
  b->set_vertex_final_to(char(238), v2);
  tran_to_text(v1->number_of, a->get_vertex_init()->number_of, char(238));
  tran_to_text(v1->number_of, b->get_vertex_init()->number_of, char(238));
  tran_to_text(a->get_vertex_final()->number_of, v2->number_of, char(238));
  tran_to_text(b->get_vertex_final()->number_of, v2->number_of, char(238));
  result->init_vertex = v1;
  result->final_vertex = v2;
  return result;
}

AFN* AFN::kleeneAFN(AFN* a){
  //cout << "Creando AFN para *" << endl;
  AFN* result = new AFN;
  /*Le agrego dos vertices/estados al AFN y le coloco transicione e para simular kleene*/
  vertex* v1 = new vertex;
  vertex* v2 = new vertex;
  v1->number_of = get_new_state();
  v2->number_of = get_new_state();
  a->set_vertex_final_to(char(238), a->get_vertex_init());
  a->set_vertex_final_to(char(238), v2);
  v1->vertex_to.push_back(make_pair(char(238), v2));
  v1->vertex_to.push_back(make_pair(char(238), a->get_vertex_init()));
  result->init_vertex = v1;
  result->final_vertex = v2;
  tran_to_text(a->get_vertex_final()->number_of, a->get_vertex_init()->number_of, char(238));
  tran_to_text(a->get_vertex_final()->number_of, v2->number_of, char(238));
  tran_to_text(v1->number_of, v2->number_of, char(238));
  tran_to_text(v1->number_of, a->get_vertex_init()->number_of, char(238));
  return result;
}

AFN* AFN::concAFN(AFN* a, AFN* b){
  //cout << "Creando AFN para ^" << endl;
  /*Uno a y b con una transicion e*/
  AFN* result = new AFN;
  b->set_vertex_final_to(char(238), a->get_vertex_init());
  result->init_vertex = b->get_vertex_init();
  result->final_vertex = a->get_vertex_final();
  tran_to_text(b->get_vertex_init()->number_of, a->get_vertex_final()->number_of, char(238));
  return result;
}
