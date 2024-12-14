// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include "mapContainer.h"
#include <iostream> // cout

using namespace std;

map<string, Symbol> symbolTable;

Symbol *insertSymbol(const string &text, int type)
{
  auto result = symbolTable.emplace(text, Symbol{type, text});

  return &(result.first->second);
}

void printSymbolTable()
{
  for (const auto &entry : symbolTable)
  {
    cout << "Symbol: " << entry.first << ", Type: " << entry.second.type << endl;
  }
}
