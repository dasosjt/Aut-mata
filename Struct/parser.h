#ifndef PARSER_H
#define PARSER_H

using namespace std;

class Parser{
  public:
    Parser();
    void Scan();
    void setNextTokens(vector< string > nextTokens0);
    void consume();
    string la();
//-->methods


  private:
    vector< string > nextTokens;
    string id;
    string $;
};

#endif
