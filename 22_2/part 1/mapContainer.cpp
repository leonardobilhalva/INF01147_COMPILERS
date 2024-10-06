// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include "mapContainer.h"
#include <iostream>

using namespace std;

map<string, Symbol> symbolTable;

void insertSymbol(const string& text, int type) {
    symbolTable[text] = Symbol();
    symbolTable[text].type = type;
    symbolTable[text].text = text;
}

Symbol* findSymbol(const string& text) {
    map<string, Symbol>::iterator it = symbolTable.find(text);
    if (it != symbolTable.end()) {
        return &(it->second); 
    }
    return nullptr; 
}

void printSymbolTable() {
    for (map<string, Symbol>::iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
        cout << "Símbolo: " << it->first << ", Tipo: " << it->second.type << endl;
    }
}
