#include "tree.h"
#include "afn.h"
#include "afdx.h"
#include "afd.h"
#include <string>
#include <iostream>
#include <iterator>

using namespace std;

int main(int argc, char *argv[]){
    vector<char> L;
    Tree* tree = new Tree();
    string exprsn;
    cout << "Enter Regular Expression " << endl;
    cin >> exprsn ;
    tree->parse(exprsn);
    cout << "AST " << endl;
    tree->display();
    L = tree->getL();
    cout << "AFN START" << endl;
    AFN* afn = new AFN();
    afn->createAFN(tree->getRoot(), L);
    cout << "AFN READY FOR TEST" << endl;
    cout << "Enter Expression " << endl;
    cin >> exprsn ;
    afn->simulationAFN(exprsn);
    AFDX* afdx = new AFDX();
    cout << "AFN TO AFD "<< endl;
    afdx->createAFDX(afn->get_vertex_init_result(), L);
    cout << "AFDX READY FOR TEST" << endl;
    cout << "Enter Expression " << endl;
    cin >> exprsn ;
    afdx->simulationAFDX(exprsn);
    afdx->minAFD();
    cout << "AST TO AFD "<< endl;
    AFD* afd = new AFD();
    afd->createAFD(tree->getRoot(), L);
    cout << "AFD READY FOR TEST" << endl;
    cout << "Enter Expression " << endl;
    cin >> exprsn ;
    afd->simulationAFD(exprsn);
    //afd->minAFD();
}
