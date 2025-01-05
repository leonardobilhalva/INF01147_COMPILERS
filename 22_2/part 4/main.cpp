// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include <iostream>       // std inputs and outputs -> cerr | cout
#include <fstream>        // file management -> file management
#include "mapContainer.h" // map container -> symbol table
#include "y.tab.h"        // bison stuff
#include "ast.hh"         // ast stuff
#include "semantic.hh"    // semantic stuff

using namespace std;

AST *root = nullptr;   // ast stuff
extern FILE *yyin;     // lexer stuff
extern AST *getRoot(); // ast stuff
extern int lineNumber;
extern int running;

extern int checkSemantic();

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    cerr << "Usage: ./etapa2 <input_file> <output_file>" << endl;
    return 1;
  }

  ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    cerr << "Error opening input file: " << argv[1] << endl;
    return 1;
  }

  yyin = fopen(argv[1], "r");
  if (!yyin)
  {
    cerr << "Error associating FILE* for lexer with file: " << argv[1] << endl;
    return 1;
  }

  if (yyparse() == 0)
  {
    // printSymbolTable();
  }
  else
  {
    cerr << "Parsing failed." << endl;
    fclose(yyin);
    return 3;
  }

  root = getRoot();
  // if (root)
  // {
  //   ofstream outputFile(argv[2]);
  //   if (!outputFile.is_open())
  //   {
  //     cerr << "Error opening output file: " << argv[2] << endl;
  //     fclose(yyin);
  //     return 3;
  //   }

  //   astPrintCode(root, outputFile);
  //   outputFile.close();
  // }
  // else
  // {
  //   cerr << "AST root is null. No code to print." << endl;
  // }

  fprintf(stderr, "Checking Semantic...\n");
  checkSemantic();
  fprintf(stderr, "No Semantic Errors.\n");

  fclose(yyin);

  cout << "Main done! File has " << lineNumber << " lines" << endl;

  return 0;
}
