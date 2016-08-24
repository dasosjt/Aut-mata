#include "tree.h"
#include <string>


int main(int argc, char *argv[]){
    Tree* tree = new Tree();
    tree->parse("(a|b(ab))a|b");
}
