#include "tree.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    Tree* tree = new Tree();
    string exprsn;
    cin >> exprsn ;
    tree->parse(exprsn);
}
