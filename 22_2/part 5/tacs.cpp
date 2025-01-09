// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include <iostream>
#include <vector>
#include "tacs.hh"
#include "mapContainer.h"

using namespace std;

const char *tacTypeStrings[] = {
    "TAC_UNKNOWN",
    "TAC_SYMBOL",
    "TAC_ADD",
    "TAC_SUB"};

TAC *tacCreate(int type, Symbol *res, Symbol *op1, Symbol *op2)
{
  TAC *newTac = (TAC *)calloc(1, sizeof(TAC));
  if (!newTac)
  {
    cerr << "Error: Failed to allocate memory for TAC." << endl;
    return nullptr;
  }

  newTac->type = type;
  newTac->res = res;
  newTac->op1 = op1;
  newTac->op2 = op2;
  newTac->prev = nullptr;
  newTac->next = nullptr;

  return newTac;
}

void tacPrint(TAC *tac)
{
  if (!tac)
    return;

  int tacTypeCount = sizeof(tacTypeStrings) / sizeof(tacTypeStrings[0]);

  cout << "TAC(";
  const char *typeString = (tac->type >= 1 && tac->type < tacTypeCount)
                               ? tacTypeStrings[tac->type]
                               : "TAC_UNKNOWN";
  cout << typeString;

  cout << ", res=" << (tac->res ? tac->res->text : "null")
       << ", op1=" << (tac->op1 ? tac->op1->text : "null")
       << ", op2=" << (tac->op2 ? tac->op2->text : "null")
       << ")" << endl;
}

TAC *tacJoin(TAC *l1, TAC *l2)
{
  if (!l1)
    return l2;
  if (!l2)
    return l1;

  TAC *tail = l2;
  while (tail->prev)
  {
    tail = tail->prev;
  }

  tail->prev = l1;
  return l2;
}

void tacPrintBackwards(TAC *tac)
{
  if (!tac)
    return;

  TAC *current = tac;
  while (current->prev)
  {
    current = current->prev;
  }

  while (current)
  {
    tacPrint(current);
    current = current->next;
  }
}

TAC *generateCode(AST *node)
{
  if (!node)
    return nullptr;

  TAC *result = nullptr;
  vector<TAC *> code(node->sons.size(), nullptr);

  for (size_t i = 0; i < node->sons.size(); ++i)
  {
    code[i] = generateCode(node->sons[i]);
  }

  switch (node->type)
  {
  case AST_SYMBOL:
    result = tacCreate(TAC_SYMBOL, node->symbol, nullptr, nullptr);
    break;
  case AST_ADD:
    result = tacJoin(tacJoin(code[0], code[1]),
                     tacCreate(TAC_ADD, makeTemp(),
                               code[0] ? code[0]->res : nullptr,
                               code[1] ? code[1]->res : nullptr));
    break;
  // case AST_ATTR:
  //   result = tacJoin(code[0],
  //                    tacCreate(TAC_COPY, node->symbol,
  //                              code[0] ? code[0]->res : nullptr, nullptr));
  //   break;
  // case AST_IFT:
  //   result = makeIfThen(code[0], code[1]);
  //   break;
  default:
    for (auto &tac : code)
    {
      result = tacJoin(result, tac);
    }
    break;
  }

  return result;
}

void tacPrintAll(TAC *tac)
{
  if (!tac)
    return;

  TAC *current = tac;
  while (current)
  {
    tacPrint(current);
    current = current->prev;
  }
}
