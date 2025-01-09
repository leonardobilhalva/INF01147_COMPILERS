// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#ifndef MAPCONTAINER_H
#define MAPCONTAINER_H

#include <string>
#include <map>

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_CHAR 2
#define SYMBOL_LIT_STRING 3
#define SYMBOL_LIT_TRUE 4
#define SYMBOL_LIT_FALSE 5
#define SYMBOL_IDENTIFIER 6

#define SYMBOL_VAR 10
#define SYMBOL_VECTOR 11
#define SYMBOL_FUNC 12
#define SYMBOL_PARAM 13

#define DATATYPE_CHAR 20
#define DATATYPE_INT 21
#define DATATYPE_BOOL 22

using namespace std;
struct Symbol
{
  int type;
  int dataType;
  string text;
};

extern map<string, Symbol> symbolTable;

Symbol *insertSymbol(const string &text, int type);
void printSymbolTable();

#endif // MAPCONTAINER_H
