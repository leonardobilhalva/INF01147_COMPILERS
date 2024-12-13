// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#ifndef MAPCONTAINER_H
#define MAPCONTAINER_H

#include <string>
#include <map>

using namespace std;
struct Symbol
{
  int type;
  string text;
};

extern map<string, Symbol> symbolTable;

Symbol *insertSymbol(const string &text, int type);
void printSymbolTable();

#endif // MAPCONTAINER_H
