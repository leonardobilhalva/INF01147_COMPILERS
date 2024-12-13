// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include <iostream>       // std inputs and outputs -> cerr | cout
#include <fstream>        // file management -> file management
#include "mapContainer.h" // map container -> symbol table
#include "y.tab.h"        // bison stuff
#include "ast.hh"         // bison stuff

using namespace std;

AST *root = nullptr;   // ast stuff
extern FILE *yyin;     // lexer stuff
extern AST *getRoot(); // ast stuff
extern int lineNumber;
extern int running;

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    cerr << "Usage: ./etapa2 <file_name>" << endl;
    return 1;
  }

  ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    cerr << "Error opening file: " << argv[1] << endl;
    return 1;
  }

  yyin = fopen(argv[1], "r");
  if (!yyin)
  {
    cerr << "Error associating FILE* for lexer" << endl;
    return 1;
  }

  if (yyparse() == 0)
  {
    printSymbolTable();
  }
  else
  {
    cerr << "Parsing failed." << endl;
    exit(3);
  }

  AST *root = getRoot();
  if (root)
  {
    std::ofstream outputFile("output.txt");
    astPrintCode(root, outputFile);
    outputFile.close();
  }
  return 0;

  cout << "Main done! File has " << lineNumber << " lines" << endl;
  fclose(yyin);

  return 0;
}
