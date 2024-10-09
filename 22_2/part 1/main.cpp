// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include <iostream> // std inputs and outputs -> cerr | cout
#include <fstream> // file management -> file management
#include <FlexLexer.h>  // c++ flex stuff
#include "mapContainer.h"  // map container -> symbol table
#include "tokens.h"

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

  FlexLexer* lexer = new yyFlexLexer(&yyin, &cout);

  while (running)
  {
    int tok = lexer->yylex(); 

    if (running == 0) break;
    switch (tok)
    {
    case KW_CHAR: cout << "Found KW_CHAR" << endl; break;
    case KW_INT: cout << "Found KW_INT" << endl; break;
    case KW_IF: cout << "Found KW_IF" << endl; break;
    case KW_THEN: cout << "Found KW_THEN" << endl; break;
    case KW_ELSE: cout << "Found KW_ELSE" << endl; break;
    case KW_WHILE: cout << "Found KW_WHILE" << endl; break;
    case KW_READ: cout << "Found KW_READ" << endl; break;
    case KW_PRINT: cout << "Found KW_PRINT" << endl; break;
    case KW_RETURN: cout << "Found KW_RETURN" << endl; break;
    case TK_IDENTIFIER: cout << "Found TK_IDENTIFIER" << endl; break;
    case LIT_INT: cout << "Found LIT_INT" << endl; break;
    case LIT_CHAR: cout << "Found LIT_CHAR" << endl; break;
    case LIT_STRING: cout << "Found LIT_STRING" << endl; break;
    case TOKEN_ERROR: cout << "Found TOKEN_ERROR" << endl; break;
    default: cout << "Found " << lexer->YYText()[0] << endl; break;
    }
  }

  printSymbolTable();
  cout << "Main done! File has " << lineNumber << " lines" << endl;

  delete lexer; 
  return 0;
}