#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>

using namespace std;

int AFD::state;
int AFD::new_id_number;
vertex* AFD::init_vertex;
vertex* AFD::final_vertex;
ofstream AFD::AFD_file;
ostringstream AFD::AFD_output_t;


AFD::AFD(){
  root = NULL;
  state = 0;
  new_id_number = 0;
  init_vertex = new vertex;
  final_vertex = new vertex;
}

void AFD::set_root(node* root){
  //cout << root->key_value << endl;
  /*Agrego el sharp y concatenación hasta arriba del arbol para saber que es el final*/
  node* fsharp = new node;
  fsharp->key_value = '#';
  fsharp->left = NULL;
  fsharp->right = NULL;
  node* nroot = new node;
  nroot->key_value = '^';
  nroot->left = root;
  nroot->right = fsharp;
  this->root = nroot;
  cout << "----- Set ID Number -----" << endl;
  set_id_number(this->root);
  cout << "----- Set Voidable -----" << endl;
  set_voidable(this->root);
  cout << "----- Set Firstpos -----" << endl;
  set_firstpos(this->root);
  cout << "----- Set Lastpos -----" << endl;
  set_lastpos(this->root);
  cout << "----- Set Nextpos -----" << endl;
  set_nextpos(this->root);
}

void AFD::createAFD(node* root, vector<char> L){
  /*Inicializo las variables para crear el AFD*/
  set_root(root);
  //cout << "----- Creating AFD from AST -----" << endl;
  stack<vector<int > > Dstates;
  //cout << "Node with key value "<< this->root->key_value << " with firstpos { ";
  //copy(this->root->firstpos.begin(), this->root->firstpos.end(), ostream_iterator<int>(cout, " "));
  //cout << "}"<< endl;
  Dstates.push(this->root->firstpos);
  init_vertex->number_of = get_new_state();
  init_vertex->afdx_set = this->root->firstpos;
  states_afd.push_back(init_vertex);
  //L.push_back('#');
  this->L = L;
  vector<char>::iterator pos = find(this->L.begin(), this->L.end(), 'e');
  if(pos != this->L.end()){
    this->L.erase(pos);
  }
  L = this->L;
  /*Agrego en Dstate los nuevos estados encontrados y en states_afd tambien, solo que en el ultimo mantengo un orden para acceder*/
  while(!Dstates.empty()){
      vector<int > S  = Dstates.top();
      vertex* v_from = new vertex;
      v_from->afdx_set = S;
      Dstates.pop();
      for(unsigned int i = 0; i<this->L.size(); i++){
        vector<int > U;
        for(unsigned int j = 0; j<S.size(); j++){
            node* temp = new node;
            //cout << "Searching id number " << S[j] << " with char '" << L[i] <<"'" <<endl;
            temp = search_node(this->root, S[j]);
            if(temp->key_value == this->L[i]){
              //cout << "Search node done.. found '" << temp->key_value << "' with id number "<< temp->id_number <<endl;
              //cout << "appending to U { " ;
              //copy(temp->nextpos.begin(), temp->nextpos.end(), ostream_iterator<int>(cout, " "));
              //cout << "}" << endl;
              for(unsigned int k = 0; k<temp->nextpos.size(); k++){
                if(find(U.begin(), U.end(), temp->nextpos[k]) == U.end()){
                  U.push_back(temp->nextpos[k]);
                } else {
                  //cout << "U already has this element: " << temp->nextpos[k] <<endl;
                }
              }
            } else {
              //cout << "Search node done.. found incorrect '" << temp->key_value << "' with id number "<< temp->id_number <<endl;
            }
        }
        sort(U.begin(), U.end());
        vertex* v_to = new vertex;
        v_to->afdx_set = U;
        if(count_if(states_afd.begin(), states_afd.end(), compare_by_afdx_set(v_to->afdx_set)) == 0){
          //cout << "New AFD state found { " ;
          //copy(U.begin(), U.end(), ostream_iterator<int>(cout, " "));
          //cout << "}"<< endl;
          v_to->number_of = get_new_state();
          states_afd.push_back(v_to);
          Dstates.push(U);
        }
        /*Coloco las transiciones segun lo que encontre en el vector states_afd*/
        vector<vertex* >::iterator it_v_from = find_if(states_afd.begin(), states_afd.end(), compare_by_afdx_set(v_from->afdx_set));
        vector<vertex* >::iterator it_v_to = find_if(states_afd.begin(), states_afd.end(), compare_by_afdx_set(v_to->afdx_set));
        int index_v_from = distance( states_afd.begin(), it_v_from );
        int index_v_to = distance( states_afd.begin(), it_v_to );
        //cout << "State index from -> " << index_v_from << endl;
        //cout << "State index to -> " << index_v_to << endl;
        //cout << "with.. " << L[i] << endl;
        /*Agrego la transiciones*/
        states_afd[index_v_from]->vertex_to.push_back(make_pair(L[i], states_afd[index_v_to]));
        tran_to_text(states_afd[index_v_from]->number_of, states_afd[index_v_to]->number_of, L[i]);
      }
  }
}

int AFD::get_new_id_number(){
  /*id number para un nodo de un arbol*/
  new_id_number+=1;
  //cout <<"new id number "<< new_id_number << endl;
  return new_id_number;
}

int AFD::get_new_state(){
  /*numero de estado para un vertex/estado del afd*/
  state+=1;
  //cout <<"new state "<< state << endl;
  return state;
}

void AFD::set_id_number(node* root){
  /*coloco el numero de id number solo si no es un operador*/
  if(root != NULL){
    if(root->key_value == '|' || root->key_value == '^' || root->key_value == '*'){
        root->id_number = 0;
    } else {
      root->id_number = get_new_id_number();
      if(root->key_value == '#'){
        final_vertex->number_of = new_id_number;
        //cout << "FINAL VERTEX ID NUMBER "<< new_id_number << endl;
      }
      //cout << root->key_value << " with id number " << root->id_number << endl;
    }
    set_id_number(root->left);
    set_id_number(root->right);
  }
}

bool AFD::set_voidable(node* root){
  if(root != NULL){
    /*coloco el voidable segun las reglas de cada operador y caracter */
    char key_value = root->key_value;
    if(key_value == '|'){
      bool temp_right = set_voidable(root->right);
      bool temp_left = set_voidable(root->left);
      if(temp_right || temp_left){
        root->voidable = true;
      } else {
        root->voidable = false;
      }
    } else if(key_value == '^'){
      bool temp_right = set_voidable(root->right);
      bool temp_left = set_voidable(root->left);
      if(temp_right && temp_left){
        root->voidable = true;
      } else {
        root->voidable = false;
      }
    } else if ( key_value == '*'){
      root->voidable = true;
      set_voidable(root->right);
      set_voidable(root->left);
    } else if ( key_value == 'e'){
      root->voidable = true;
    } else {
      root->voidable = false;
    }
    if (root->id_number == 0){
      //cout << root->key_value << " is voidable " << root->voidable << endl;
    } else {
      //cout << root->key_value << " with id number " << root->id_number << " is voidable " << root->voidable << endl;
    }

    return root->voidable;
  }
}

vector<int> AFD::set_firstpos(node* root){
  if(root != NULL){
    /*coloco el firstpos segun las reglas de cada operador y caracter */
    char key_value = root->key_value;
    if (key_value == '|'){
      vector<int> temp_left = set_firstpos(root->left);
      vector<int> temp_right = set_firstpos(root->right);
      root->firstpos = temp_left;
      root->firstpos.insert(root->firstpos.end(), temp_right.begin(), temp_right.end());
    } else if (key_value == '^'){
      vector<int> temp_left = set_firstpos(root->left);
      vector<int> temp_right = set_firstpos(root->right);
      if(root->left->voidable){
        root->firstpos = temp_left;
        root->firstpos.insert(root->firstpos.end(), temp_right.begin(), temp_right.end());
      } else {
        root->firstpos = temp_left;
      }
    } else if (key_value == '*'){
      vector<int> temp_right = set_firstpos(root->right);
      root->firstpos = temp_right;
    } else {
      root->firstpos.push_back(root->id_number);
    }
    if (root->id_number == 0){
      //cout << root->key_value << " with firstpos { ";
    } else {
      //cout << root->key_value << " with id number " << root->id_number << " with firstpos { ";
    }
    //copy(root->firstpos.begin(), root->firstpos.end(), ostream_iterator<int>(cout, " "));
    //cout << "}"<< endl;
    return root->firstpos;
  }
}

vector<int> AFD::set_lastpos(node* root){
  if(root != NULL){
    /*coloco el lastpos segun las reglas de cada operador y caracter */
    char key_value = root->key_value;
    if (key_value == '|'){
      vector<int> temp_left = set_lastpos(root->left);
      vector<int> temp_right = set_lastpos(root->right);
      root->lastpos = temp_left;
      root->lastpos.insert(root->lastpos.end(), temp_right.begin(), temp_right.end());
    } else if (key_value == '^'){
      vector<int> temp_left = set_lastpos(root->left);
      vector<int> temp_right = set_lastpos(root->right);
      if(root->right->voidable){
        root->lastpos = temp_right;
        root->lastpos.insert(root->lastpos.end(), temp_left.begin(), temp_left.end());
      } else {
        root->lastpos = temp_right;
      }
    } else if (key_value == '*'){
      vector<int> temp_right = set_lastpos(root->right);
      root->lastpos = temp_right;
    } else {
      root->lastpos.push_back(root->id_number);
    }
    if (root->id_number == 0){
      //cout << root->key_value << " with lastpos { ";
    } else {
      //cout << root->key_value << " with id number " << root->id_number << " with lastpos { ";
    }
    sort(root->lastpos.begin(), root->lastpos.end());
    //copy(root->lastpos.begin(), root->lastpos.end(), ostream_iterator<int>(cout, " "));
    //cout << "}"<< endl;
    return root->lastpos;
  }
}

void AFD::set_nextpos(node* root){
  if(root != NULL){
    /*coloco el nextpos segun las reglas de cada operador y caracter */
    if(root->key_value == '^'){
      //cout << "NODE ^" << endl;
      for(unsigned int i = 0; i<root->left->lastpos.size(); i++){
        node* temp = search_node(root->left, root->left->lastpos[i]);
        temp->nextpos.insert(temp->nextpos.end(), root->right->firstpos.begin(), root->right->firstpos.end());
        sort(temp->nextpos.begin(), temp->nextpos.end());
        //cout << "Node with key value "<< temp->key_value << " with id number " << temp->id_number << " with nextpos { ";
        //copy(root->right->firstpos.begin(), root->right->firstpos.end(), ostream_iterator<int>(cout, " "));
        //cout << "}"<< endl;
      }
    } else if (root->key_value == '*'){
      //cout << "NODE *" << endl;
      for(unsigned int i = 0; i<root->lastpos.size(); i++){
        node* temp = search_node(root, root->lastpos[i]);
        temp->nextpos.insert(temp->nextpos.end(), root->firstpos.begin(), root->firstpos.end());
        sort(temp->nextpos.begin(), temp->nextpos.end());
        //cout << "Node with key value "<< temp->key_value << " with id number " << temp->id_number << " with nextpos { ";
        //copy(temp->nextpos.begin(), temp->nextpos.end(), ostream_iterator<int>(cout, " "));
        //cout << "}"<< endl;
      }
    }
    set_nextpos(root->left);
    set_nextpos(root->right);
  }
}

node* AFD::search_node(node* root, int id_number){
  if(root != NULL){
    /*busco un nodo en cada rama, si no esta en una rama devuelvo un NULL. Si esta entonces devuelve esa rama*/
    //cout << "Node key value '"<< root->key_value << "' Node id number " << root->id_number << endl;
    //cout << " ... searching for " << id_number << endl;
    if(root->key_value == '|' || root->key_value == '^'){
      node* temp_left = new node;
      temp_left = search_node(root->left, id_number);
      if(temp_left != NULL){
        return temp_left;
      } else if (search_node(root->right, id_number) != NULL){
        return search_node(root->right, id_number);
      }
    } else if (root->key_value == '*'){
      node* temp_right = new node;
      temp_right = search_node(root->right, id_number);
      if(temp_right != NULL){
        return temp_right;
      }
    } else {
      if(root->id_number == id_number){
        return root;
      } else {
        return NULL;
      }
    }
  }
}

void AFD::tran_to_text(int from, int to, char a){
  ostringstream temp;
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  temp << "(" << from << ", " << a << ", " << to << ")";
  AFD_output_t << temp.str();
}

string AFD::states_to_text(){
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  ostringstream temp;
  temp << "{";
  for(unsigned int i = 1; i<=state; i++){
    temp << i << ", ";
  }
  temp << "}";
  return temp.str();
}

string AFD::symbols_to_text(){
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  ostringstream temp;
  temp << "{";
  copy(this->L.begin(), this->L.end(), ostream_iterator<char>(temp, ", "));
  temp << "}";
  return temp.str();
}

string AFD::final_to_text(){
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  ostringstream temp;
  temp << "{";
  for(unsigned int i = 0; i<states_afd.size(); i++){
    bool final_state_t = false;
    for(unsigned int j = 0; j<states_afd[i]->afdx_set.size(); j++){
      if(states_afd[i]->afdx_set[j] == final_vertex->number_of){
        final_state_t = true;
      }
      if(final_state_t){
        temp << states_afd[i]->number_of << ", ";
      }
    }
  }
  temp << "}";
  return temp.str();
}

string AFD::init_to_text(){
  /*agrego a temp en forma ordena la informacion brindada y la devuelvo como string*/
  ostringstream temp;
  temp << "{" << init_vertex->number_of << "}";
  return temp.str();
}

void AFD::simulationAFD(string exprsn){
  /*Agrego un caracter que denota que es el final de leer caracteres, en este caso es el f*/
  vector<vertex*> S;
  vector<char> expression(exprsn.begin(), exprsn.end());
  expression.push_back('f');
  S.push_back(init_vertex);
  char c = expression[0];
  expression.erase(expression.begin());
  bool all_belongs = true;
  /*mientras no sea f... */
  while(c!='f'){
    /*Hago el move correspondiente*/
    if (find(this->L.begin(), this->L.end(), c) != this->L.end()){
      S = move(S, c);
      c = expression[0];
      expression.erase(expression.begin());
    } else {
      S = move(S, c);
      c = expression[0];
      expression.erase(expression.begin());
      all_belongs = false;
    }
  };
  final_state = false;
  /*Si el estado resultante no tiene el estado final, entonces respondo NO; pero si lo tiene entonces SI*/
  for(unsigned int i = 0; i < S[0]->afdx_set.size(); i++){
    if (S[0]->afdx_set[i] == final_vertex->number_of && all_belongs){
      final_state = true;
    };
  }
  if(final_state){
    cout << "YES" << endl;
  } else {
    cout << "NO" << endl;
  }
  /*Guardo en el archivo segun la rubrica*/
  AFD_file.open("AFD.txt", ios::out);
  if (AFD_file.is_open()) {
    AFD_file << "ESTADOS = ";
    AFD_file << states_to_text() << endl;
    AFD_file << "SIMBOLOS = ";
    AFD_file << symbols_to_text() << endl;
    AFD_file << "INICIO = ";
    AFD_file << init_to_text() << endl;
    AFD_file << "ACEPTACION = ";
    AFD_file << final_to_text() << endl;
    AFD_file << "TRANSICION = ";
    AFD_file << AFD_output_t.str() << endl;
  }
  AFD_file.close();
}

void AFD::minAFD(){
  vector<vertex* > F;
  vector<vertex* > S_F;
  vector<vector<vertex* > > PI, nPI, sPI;
  for(unsigned int i = 0; i < states_afd.size(); i++){
    bool final_state = false;
    for(unsigned int j = 0; j < states_afd[i]->afdx_set.size(); j++){
      if (states_afd[i]->afdx_set[j] == final_vertex->number_of){
        final_state = true;
      };
    }
    if(final_state == true){
      F.push_back(states_afd[i]);
    } else {
      S_F.push_back(states_afd[i]);
    }
  }
  PI.push_back(F);
  PI.push_back(S_F);
  //cout << "Size of F " << F.size() << endl;
  //cout << "Size of S-F " << S_F.size() << endl;
  //cout << "Size of PI " << PI.size() << endl;
  nPI = PI;
  while(nPI != sPI){
    //cout << "Size NPI " <<nPI.size() << endl;
    //cout << "Size sPI " << sPI.size() << endl;
    vector<vertex* > belongsMV;
    vector<vertex* > belongsnMV;
    sPI = PI;
    PI = nPI;
    nPI.clear();
    for(unsigned int k = 0; k<PI.size(); k++){ //Revisando grupo por grupo
      if(PI[k].size() > 1){ //Si es mayor que uno, entonces puede dividirse
        vector<int > control(PI[k].size()); //vector de control
        vector<vector<vector<vertex* > > > belongsMatrix(PI[k].size(), vector<vector<vertex* > >(L.size())); //matriz de pertenencia
        for(unsigned int i = 0; i<PI[k].size(); i++){ //Recorremos segun el grupo, ABCD
          for(unsigned int c = 0; c<L.size(); c++){ //Recorremos segun caracter, a b
            vector<vertex* > temp_PIKI;
            temp_PIKI.push_back(PI[k][i]);
            belongsMatrix[i][c] = searchForBelong(PI, move(temp_PIKI, L[c])[0]->number_of); //buscamos al grupo que pertenece y lo guardamos en la matriz
          }
        }
        vector<vector<vertex* > > G(PI[k].size()); //creamos todos los grupos posibles
        for(unsigned int i = 0; i<PI[k].size(); i++){ //Verfificamos para A
          if(control[i] != 1){ //Si el control aun es 0, entonces no hemos hecho ese grupo
            //cout << "Estado " << i << endl;
            if(find(G[i].begin(), G[i].end(), PI[k][i]) == G[i].end()){
                G[i].push_back(PI[k][i]); //Metemos a A
            }
            control[i] = 1; //Marcamos A
            for(unsigned int j = i+1; j<PI[k].size(); j++){ //Verificamos B, C, D
              if(control[j] != 1){
                //cout << "     con Estado "<< j << endl;
                if(belongsMatrix[i] == belongsMatrix[j]){ // Si es igual a A, entonces pertenece al mismo grupo
                  //cout << "       Estado " <<i << " es igual a  Estado " << j << endl;
                  if(find(G[i].begin(), G[i].end(), PI[k][j]) == G[i].end()){
                      G[i].push_back(PI[k][j]); //Metemos a B
                  }
                  control[j] = 1; //Marcamos B
                }
              }
            }
          }
        }
        //cout << "Ingresamos los siguientes grupos: " << endl;
        for(unsigned int g = 0; g<G.size(); g++){
          if(!G[g].empty()){
              //cout << "   Metemos grupo " << g << endl;
              if(find(nPI.begin(), nPI.end(), G[g]) == nPI.end()){
                  nPI.push_back(G[g]);
              }
          }
        }
      } else { //si es 1 entonces simplemente lo agrego
        nPI.push_back(PI[k]);
      }
    }
  }
  //cout << "Last size sPI " << sPI.size() << endl;
  cout << "MIN AFD" << endl;
  for(unsigned int i = 0; i<sPI.size(); i++){
    cout << "Group " << i << endl;
    for(unsigned int j = 0; j<sPI[i].size(); j++){
      cout << "   " <<sPI[i][j]->number_of << endl;
    }
  }
  if(final_state){
    cout << "MIN AFD - YES" << endl;
  } else {
    cout << "MIN AFD - NO" << endl;
  }
}

vector<vertex* > AFD::searchForBelong(vector<vector<vertex* > > PI, int state){
  for(unsigned int i = 0; i<PI.size(); i++){
    for(unsigned int j = 0; j<PI[i].size(); j++){
      if(PI[i][j]->number_of == state){
        return PI[i];
      }
    }
  }
}

vector<vertex* > AFD::move(vector<vertex* > v, char c){
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
