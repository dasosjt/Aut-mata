#ifndef AFD_H
#define AFD_H


using namespace std;

/**
* Clase AFD generada a partir del AST(Abstract Syntax Tree)
* por lo tanto puede aceptar cadenas no vacias y solamente se encuentra en un estado a la vez
*/

class AFD{
  public:
    /** Constructor */
    AFD();
    /** Colocamos el root a partir de un puntero del arbol.
    \param root es un puntero de nodo que pertenece al arbol
    \return void
    */
    void set_root(node* root);
    /** Obtenemos un nuevo id number para los nodos del arbol
    \return un integer para el nodo
    */
    int get_new_id_number();
    /** Obtenemos un nuevo estado para el AFD
    \return un integer para el vertice/estado
    */
    int get_new_state();
    /** Colocamos a cada nodo del arbol un id number
    \param root es un puntero de nodo que pertenece al arbol
    \return void
    */
    void set_id_number(node* root);
    /** Colocamos a cada nodo del arbol si es voidable
    \param root es un puntero de nodo que pertenece al arbol
    \return bool que representa 1 si es voidable
    */
    bool set_voidable(node* root);
    /** Colocamos a cada nodo del arbol su firstpos
    \param root es un puntero de nodo que pertenece al arbol
    \return un vector de int que representa su firstpos
    */
    vector<int> set_firstpos(node* root);
    /** Colocamos a cada nodo del arbol su lastpos
    \param root es un puntero de nodo que pertenece al arbol
    \return un vector de int que representa su lastpos
    */
    vector<int> set_lastpos(node* root);
    /** Buscamos en el arbol a partir de un nodo un nodo con cierto id number
    \param root es un puntero de nodo que pertenece al arbol
    \param id_number el id number que estamos buscando
    \return el puntero del nodo que estamos buscando
    */
    node* search_node(node* root, int id_number);
    /** Colocamos a cada nodo del arbol su nextpos
    \param root es un puntero de nodo que pertenece al arbol
    \return void
    */
    void set_nextpos(node* root);
    /** Metodo que crea el AFD
    \param root es un puntero de un nodo del arbol
    \param L es el lenguaje que se detecto en el arbol
    \return void
    */
    void createAFD(node* root, vector<char> L);
    /** Metodo que crea el min AFD
    \return void
    */
    void minAFD();
    /** Metodo que simula el AFN a partir de una expresion
    \param exprsn es un string que se verificara si es o no es aceptada por el AFN
    \return void
    */
    void simulationAFD(string exprsn);
    /** Movimiento move
    \param v es un conjunto de vertices/estados a los cuales se les aplica el movimiento move
    \param c es el caracter con el cual se esta realizando el movimiento
    \return un grupo de vertices/estados de resultado del movimiento move
    */
    vector<vertex* > move(vector<vertex* > v, char c);
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
    /** Busca en PI hacia donde pertence segun un numero de estado
    \param PI el grupo de conjunto de estados
    \param state el estado que se esta buscando
    \return el resultado en forma de grupo
    */
    vector<vertex* > searchForBelong(vector<vector<vertex* > > PI, int state);

  private:
    node* root;
    static vertex* init_vertex; /**< representacion del estado inicial */
    static vertex* final_vertex; /**< representacion del estado final */
    static int new_id_number; /**< representacion del id number actual */
    static int state; /**< representacion del estado actual */
    vector<vertex* > states_afd; /**< representacion de los estados actuales */
    vector<char> L; /**< representacion del lenguaje aceptado */
    static ofstream AFD_file; /**< genera el archivo de texto */
    static ostringstream AFD_output_t; /**< guardar en memoria las transiciones antes de guardarlas en texto */
    bool final_state; /**< guardar en memoria la aceptacion */
    node* temp_search_node_left;
    node* temp_search_node_right;

};

#endif
