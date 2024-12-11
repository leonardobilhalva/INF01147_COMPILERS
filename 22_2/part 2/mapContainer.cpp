// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include "mapContainer.h"
#include <iostream> // cout

using namespace std;

map<string, Symbol> symbolTable;

void insertSymbol(const string &text, int type)
{
  symbolTable[text] = {type, text};
}

void printSymbolTable()
{
  for (const auto &entry : symbolTable)
  {
    cout << "Symbol: " << entry.first << ", Type: " << entry.second.type << endl;
  }
}
