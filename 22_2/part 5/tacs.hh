// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#ifndef TACS_HEADER
#define TACS_HEADER

#include "mapContainer.h"

enum TACType
{
  TAC_SYMBOL = 1,
  TAC_ADD,
  TAC_SUB
};

#include "ast.hh"

struct TAC
{
  int type;
  Symbol *res;
  Symbol *op1;
  Symbol *op2;
  TAC *prev;
  TAC *next;
};

TAC *tacCreate(int type, Symbol *res, Symbol *op1, Symbol *op2);
TAC *tacJoin(TAC *l1, TAC *l2);
TAC *generateCode(AST *node);
void tacPrint(TAC *tac);
void tacPrintAll(TAC *tac);
void tacPrintBackwards(TAC *tac);

#endif
