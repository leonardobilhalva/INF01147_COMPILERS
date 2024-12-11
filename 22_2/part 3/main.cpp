#include <iostream>       // std inputs and outputs -> cerr | cout
#include <fstream>        // file management -> file management
#include "mapContainer.h" // map container -> symbol table
#include "y.tab.h"        // bison stuff

extern FILE *yyin; // lexer stuff
using namespace std;

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
    cout << "Main done! File has " << lineNumber << " lines" << endl;
  }
  else
  {
    cerr << "Parsing failed." << endl;
  }

  fclose(yyin);

  return 0;
}
