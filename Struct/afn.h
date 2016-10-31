#ifndef AFN_H
#define AFN_H


#include <string>
#include <utility>
#include <vector>
#include <stack>

using namespace std;

/**
* Estructura que representa los estados de un automata, puede ser utilizado en cualquier AFD/AFN generado.
*/
struct vertex{
  /*@{*/
  string token_id;
  int number_of; /**< representacion del estado. */
  vector<int> afdx_set; /**< subconjunto de estados que se crea de AFN -> AFDX */
  vector< pair<char, vertex*> > vertex_to; /**< vector de pares, que contiene el caracter para moverse y el puntero al vertex hacia donde se dirige */
  /*@}*/
};

/**
* Clase AFN generada a partir del algoritmo de Thompson un Automata Finito no determinista,
* por lo tanto puede aceptar cadenas epsilon y estar al mismo tiempo en varios estados.
*/
class AFN{
  public:
    /** Constructor */
    AFN();
    /** Movimiento eclosure
    \param v es un conjunto de vertices/estados a los cuales se les aplica el movimiento eclosure
    \return un grupo de vertices/estados de resultado del movimiento eclosure
    */
    void newfi_vertex();

    vector<vertex*> eclosure(vector<vertex*> v);
    /** Movimiento move
    \param v es un conjunto de vertices/estados a los cuales se les aplica el movimiento move
    \param c es el caracter con el cual se esta realizando el movimiento
    \return un grupo de vertices/estados de resultado del movimiento move
    */
    vector<vertex*> move(vector<vertex* > v, char c);
    /** Metodo que crea el AFN
    \param root es un puntero de un nodo del arbol
    \param L es el lenguaje que se detecto en el arbol
    \return void
    */
    void createAFN(node* root, vector<char> L);
    /** Metodo que recrea el patron de visitador, siguiendo siempre el lado derecho del arbol primero.
    \param root es un puntero de un nodo del arbol
    \return AFN* es el puntero del resultado de AFN
    */
    AFN* visitAST(node* root);
    /** Metodo que genera el paso base del algoritmo de Thompson
    \param current es un puntero de un nodo del arbol
    \return AFN* es el puntero del resultado de AFN base
    */
    AFN* base(node* current);
    /** Metodo que genera el paso or del algoritmo de Thompson
    \param a es un AFN* necesario para el algoritmo
    \param b es un AFN* necesario para el algoritmo
    \return AFN* es el puntero del resultado de AFN or
    */
    AFN* orAFN(AFN* a, AFN* b);
    /** Metodo que genera el paso kleene del algoritmo de Thompson
    \param a es un AFN* necesario para el algoritmo
    \return AFN* es el puntero del resultado de AFN kleene
    */
    AFN* kleeneAFN(AFN* a);
    /** Metodo que genera el paso concatenacion del algoritmo de Thompson
    \param a es un AFN* necesario para el algoritmo
    \param b es un AFN* necesario para el algoritmo
    \return AFN* es el puntero del resultado de AFN concatenacion
    */
    AFN* concAFN(AFN* a, AFN* b);
    /** Metodo que obtiene el primer vertice/estado del AFN resultado
    \return vertex* que representa el primer vertice/estado del AFN resultado
    */
    vertex* get_vertex_init_result();
    /** Metodo que obtiene el primer vertice/estado del AFN actual
    \return vertex* que representa el ultimo vertice/estado del AFN actual
    */
    vertex* get_vertex_init();
    /** Metodo que obtiene el ultimo vertice/estado del AFN actual
    \return vertex* que representa el ultimo vertice/estado del AFN actual
    */
    vertex* get_vertex_final();
    /** Metodo que coloca al ultimo vertice un movimiento con el caracter e hacia el vertice/estado v
    \param e es un caracter que nos inidica que movimiento se esta realizando
    \param v es el vertice/estado al cual se le esta aplicando el movimiento
    \return void
    */
    void set_vertex_final_to(char e, vertex* v);
    /** Metodo que simula el AFN a partir de una expresion
    \param exprsn es un string que se verificara si es o no es aceptada por el AFN
    \return void
    */
    void simulationAFN(string exprsn);
    /** Traduce los transiciones a texto
    \param from es es un vertice/estado desde donde se realiza la transicion
    \param to es es un vertice/estado hacia donde se realiza la transicion
    \param a es el caracter que realiza esa transicion
    \return void
    */
    void tran_to_text(int from, int to, char a);
    /** Traduce los estados a texto
    \return el resultado en forma de string
    */
    string states_to_text();
    /** Traduce los simbolos a texto
    \return el resultado en forma de string
    */
    string symbols_to_text();
    /** Traduce los estados finales a texto
    \return el resultado en forma de string
    */
    string final_to_text();
    /** Traduce los estados iniciales a texto
    \return el resultado en forma de string
    */
    string init_to_text();
    /** Metodo que genera un nuevo estado
    \return un nuevo estado integer
    */
    int get_new_state();

  private:
    vertex* init_vertex; /**< representacion del estado inicial */
    vertex* final_vertex; /**< representacion del estado final */
    AFN* result; /**< representacion del AFN resultado */
    stack<vertex* > actual_states; /**< representacion de los estados actuales */
    stack<vertex* > new_states; /**< representacion de los nuevos estados */
    static int state; /**< representacion del estado actual */
    static ofstream AFN_file; /**< genera el archivo de texto */
    static ostringstream AFN_output_t; /**< guardar en memoria las transiciones antes de guardarlas en texto */
    vector<char> L; /**< representacion del lenguaje aceptado */
};

#endif
