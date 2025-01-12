// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include "mapContainer.h"
#include <iostream> // cout

using namespace std;

map<string, Symbol> symbolTable;

Symbol *insertSymbol(const string &text, int type)
{
  auto it = symbolTable.find(text);
  if (it != symbolTable.end())
  {
    return &(it->second);
  }

  symbolTable[text] = Symbol{type, -1, text};
  Symbol *symbol = &symbolTable[text];

  switch (type)
  {
  case SYMBOL_LIT_INT:
    symbol->dataType = DATATYPE_INT;
    break;
  case SYMBOL_LIT_CHAR:
    symbol->dataType = DATATYPE_CHAR;
    break;
  // case SYMBOL_LIT_TRUE: // duno if scanner should have true and false
  // case SYMBOL_LIT_FALSE:
  //   symbol->dataType = DATATYPE_BOOL;
  //   break;
  case SYMBOL_IDENTIFIER:
    symbol->dataType = -1;
    break;
  case SYMBOL_VAR:
    symbol->dataType = -1;
    break;
  case SYMBOL_LIT_STRING:
    symbol->dataType = -1;
    break;
  case SYMBOL_LABEL:
    symbol->dataType = -1;
    break;
  default:
    cerr << "Warning: Unknown symbol type for text '" << text << "'." << endl;
    symbol->dataType = -1;
    break;
  }

  return symbol;
}

void printSymbolTable()
{
  for (const auto &entry : symbolTable)
  {
    cout << "Symbol: " << entry.first << ", Type: " << entry.second.type << endl;
  }
}

Symbol *makeTemp()
{
  static int serial = 0;
  string tempName = "temp_" + to_string(serial++);
  return insertSymbol(tempName, SYMBOL_VAR);
}

Symbol *makeLabel()
{
  static int serial = 0;
  string labelName = "label_" + to_string(serial++);
  return insertSymbol(labelName, SYMBOL_LABEL);
}
