#ifndef TREE_H
#define TREE_H

#include <string>
#include <stack>
#include <vector>


using namespace std;

/**
* Estructura que representa los nodos de un AST(abstract syntax tree)
*/
struct node{
  /*@{*/
  char key_value; /**< representacion del valor dentro del nodo */
  node *left; /**< representacion del puntero de un nodo como un hijo izquierdo */
  node *right; /**< representacion del puntero de un nodo como un hijo derecho */
  node *parent; /**< representacion del puntero de un nodo como un padre */
  int id_number; /**< representacion del numbero brindado para la construccion de Tree -> AFD */
  bool voidable; /**< representacion del voidable brindado para la construccion de Tree -> AFD */
  vector<int> firstpos; /**< representacion del conjunto firstpos para la construccion de Tree -> AFD */
  vector<int> lastpos; /**< representacion del conjunto lastpos para la construccion de Tree -> AFD */
  vector<int> nextpos; /**< representacion del conjunto nextpos para la construccion de Tree -> AFD */
  /*@}*/
};

/**
* Clase Tree generada a partir de una expresion regular
*/
class Tree{
  public:
    /** Constructor */
    Tree();
    /** Metodo que genera el arbol a partir de la expresion
    \param exprsn es una cadena de caracteres a arbol
    \return void
    */
    void parse(string exprsn);
    /** Metodo que genera el arbol a partir de la expresion
    \return void
    */
    void parseToTree();
    /** Metodo que imprime el arbol generado
    \param root es un puntero a un nodo para la impresion del mismo
    \param level es un numero que ayuda a la impresion revursiva del arbol, brindando la distancia necesaria para su correcta representacion
    \return un integer segun la prioridad del caracter ingresado
    */
    void printLevel(node* root, int level);
    void display();
    /** Metodo que devuelve la prioridad segun cierto operador
    \param a es un caracter operacion
    \return un integer segun la prioridad del caracter ingresado
    */
    int priority(char a);
    /** Metodo que devuelve el root del arbol
    \return puntero del nodo root
    */
    node* getRoot();
    /** Metodo que devuelve el Lenguaje aceptado
    \return vector de caracteres
    */
    vector<char > getL();
    void printExpression(string exprsn);
    bool isoneDigit(char c);

  private:
    stack<char> expressionRPN; /**< stacke de expresiones que no son operaciones */
    stack<char> operations; /**< stack de operaciones que sirve para la construccion */
    stack<char> expression; /**< stack de caracteres que sirve para la construccion */
    stack<node*> nodes; /**< stack que contiene los nodos en medida que se construye el arbol*/
    node* root; /**< representacion del nodo root */
    vector<char> L; /**< representacion del lenguaje aceptado */

};

#endif
