#include "tree.h"
#include "afn.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    Tree* tree = new Tree();
    string exprsn;
    cout << "Enter Regular Expression " << endl;
    cin >> exprsn ;
    tree->parse(exprsn);
    cout << "AST " << endl;
    tree->display();
    cout << "AFN START" << endl;
    AFN* afn = new AFN();
    afn->createAFN(tree->getRoot());
    cout << "AFN READY FOR TEST" << endl;
    cout << "Enter Expression " << endl;
    cin >> exprsn ;
    afn->simulationAFN(exprsn);
}
