#ifndef SCANNER_H
#define SCANNER_H

#include <utility>
#include <vector>

class Scanner{
  public:
    Scanner();
    void Parse();

  private:
    AFN* compiler_AFN;
    vector<vertex* > AFN_lineal;
    vertex* init_vertex;
};

#endif
