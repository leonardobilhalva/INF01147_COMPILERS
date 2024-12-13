// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#ifndef AST_HEADER
#define AST_HEADER

#include "mapContainer.h"
#include <vector>

using namespace std;

enum astType
{
  AST_PROGRAM,
  AST_LIST_GLOBAL_VAR_DEC,
  AST_LIST_GLOBAL_FUNC_DEC,
  AST_VAR_DEC,
  AST_VEC_NULL_DEC,
  AST_VEC_DEC,
  AST_VEC_PARAMS,
  AST_FUNCTION_DEC,
  AST_FUNCTION_DEC_PARAMS,
  AST_NONEMPTY_FUNCTION_DEC_PARAMS,
  AST_BLOCK,
  AST_LCMD,
  AST_ASSIGN,
  AST_ASSIGN_VEC,
  AST_PRINT,
  AST_READ,
  AST_RETURN,
  AST_IF,
  AST_IFELSE,
  AST_WHILE,
  AST_NONEMPTY_LIST_PRINT_ARGS_STRING,
  AST_NONEMPTY_LIST_PRINT_ARGS_EXPR,
  AST_LIST_PRINT_ARGS_STRING,
  AST_LIST_PRINT_ARGS_EXPR,
  AST_PARENS,
  AST_VEC_CALL,
  AST_ADD,
  AST_SUB,
  AST_MUL,
  AST_DIV,
  AST_GTR,
  AST_LSS,
  AST_AND,
  AST_OR,
  AST_EQ,
  AST_NEG,
  AST_FUNC_CALL,
  AST_FUNC_CALL_PARAMS,
  AST_NONEMPTY_FUNC_CALL_PARAMS,
  AST_SYMBOL,
  AST_KW_CHAR,
  AST_KW_INT
};

struct AST
{
  int type;
  Symbol *symbol;
  vector<AST *> sons;

public:
  AST(int t, vector<AST *> v, Symbol *s = 0) : type(t), symbol(s), sons(v) {}
};

void astPrint(AST *node, int level);
void astPrintCode(AST *node, ostream &output);

#endif
