#ifndef AFDX_H
#define AFDX_H


#include <string>
#include <vector>
#include <stack>

using namespace std;

/**
* Estructura que compara los estados/vertices segun su numero
*/
struct compare_by_number_of {
    bool operator()(vertex const * a, vertex const * b) {
        return a->number_of < b->number_of;
    }
};

/**
* Estructura que compara los estados/vertices segun el conjunto de estados que se encuentran en su adfx_set
*/
struct compare_by_afdx_set{
    explicit compare_by_afdx_set(const vector<int>& vint) : afdx_set(vint) {}
    bool operator () (const vertex* v) const{
        return v->afdx_set == afdx_set;
    }
    vector<int> afdx_set;
};

/**
* Clase AFDX generada a partir de un AFN, un Automata Finito no determinista,
* por lo tanto puede aceptar cadenas epsilon y estar al mismo tiempo en varios estados.
*/
class AFDX{
  public:
    /** Constructor */
    AFDX();
    /** Metodo que crea el AFD
    \param v1 es el vertice inicial del AFN
    \param L es el lenguaje que se detecto en el arbol
    \return void
    */
    void createAFDX(vertex* v1, vector<char> L);
    /** Metodo que crea la construccion de subconjuntos
    \param v1 es el vertice inicial del AFN
    \param L es el lenguaje que se detecto en el arbol
    \return devuelve un puntero AFDX resultante
    */
    AFDX* subset_const(vertex* v1, vector<char> L);
    /** Metodo que obtiene el primer vertice/estado del AFN actual
    \return vertex* que representa el ultimo vertice/estado del AFN actual
    */
    vertex* get_vertex_init();
    /** Metodo que obtiene el ultimo vertice/estado del AFN actual
    \return vertex* que representa el ultimo vertice/estado del AFN actual
    */
    vertex* get_vertex_final();
    /** Movimiento eclosure
    \param v es un conjunto de vertices/estados a los cuales se les aplica el movimiento eclosure
    \return un grupo de vertices/estados de resultado del movimiento eclosure
    */
    vector<vertex*> eclosure(vector<vertex*> v);
    /** Movimiento move
    \param v es un conjunto de vertices/estados a los cuales se les aplica el movimiento move
    \param c es el caracter con el cual se esta realizando el movimiento
    \return un grupo de vertices/estados de resultado del movimiento move
    */
    vector<vertex*> move(vector<vertex* > v, char c);
    /** A partir de un conjunto se crea un vertice/estados
    \param v es un conjunto de vertices/estados a los cuales se les quiere crear un conjunto
    \return devuelve un vertice que contiene en su interior a v
    */
    vertex* subset_to_vertex(vector<vertex* > v);
    /** Metodo que obtiene el primer vertice/estado del AFN actual
    \return vertex* que representa el ultimo vertice/estado del AFN actual
    */
    int get_new_state();
    /** Metodo que simula el AFDX a partir de una expresion
    \param exprsn es un string que se verificara si es o no es aceptada por el AFN
    \return void
    */
    void simulationAFDX(string exprsn);
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
    /** Metodo que crea el min AFD
    \return void
    */
    void minAFD();
    /** Busca en PI hacia donde pertence segun un numero de estado
    \param PI el grupo de conjunto de estados
    \param state el estado que se esta buscando
    \return el resultado en forma de grupo
    */
    vector<vertex* > searchForBelong(vector<vector<vertex* > > PI, int state);

  private:
    AFDX* result; /**< representacion del AFDX resultado */
    vector<vertex* > states_afdx; /**< representacion de los estados resultantes */
    stack<vertex* > actual_states; /**< representacion de los estados actuales */
    stack<vertex* > new_states; /**< representacion de los nuevos estados */
    static int state; /**< representacion del estado actual */
    static vertex* init_vertex; /**< representacion del estado inicial */
    static vertex* final_vertex; /**< representacion del estado final */
    vector<char> L;  /**< representacion del lenguaje aceptado */
    static ofstream AFDX_file; /**< genera el archivo de texto */
    static ostringstream AFDX_output_t; /**< guardar en memoria las transiciones antes de guardarlas en texto */
    bool final_state; /**< guardar en memoria la aceptacion */
};

#endif
