// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include "mapContainer.h"
#include <iostream> // cout

using namespace std;

map<string, Symbol> symbolTable;

void insertSymbol(const string& text, int type) {
    symbolTable[text] = Symbol();
    symbolTable[text].type = type;
    symbolTable[text].text = text;
}

void printSymbolTable() {
    for (map<string, Symbol>::iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
        cout << "Symbol: " << it->first << ", Type: " << it->second.type << endl;
    }
}
