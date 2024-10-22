// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include <iostream> // std inputs and outputs -> cerr | cout
#include <fstream> // file management -> file management
#include <FlexLexer.h>  // c++ flex stuff
#include "mapContainer.h"  // map container -> symbol table
#include "lexer.hh"  // lexer class - https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison
#include "y.tab.hpp"  // c++ bison stuff

using namespace std;

extern int lineNumber;
extern int running;

int main(int argc, char **argv)
{
  if (argc < 2)
  {
      cerr << "Call: ./a.out file_name" << endl;
      return 1;
  }

  ifstream yyin(argv[1]);
  if (!yyin.is_open())
  {
      cerr << "Error opening file: " << argv[1] << endl;
      return 1;
  }

  Lexer lexer;
  lexer.switch_streams(&yyin, &cout); // function from FlexLexer

  yy::parser parser(lexer);
  parser();

  printSymbolTable();
  cout << "Main done! File has " << lineNumber << " lines" << endl;

  return 0;
}