// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include <iostream>
#include <vector>
#include <cstdlib>
#include "tacs.hh"
#include "mapContainer.h"

using namespace std;

const char *tacTypeStrings[] = {
    "TAC_UNKNOWN",
    "TAC_SYMBOL",
    "TAC_ADD",
    "TAC_SUB",
    "TAC_MUL",
    "TAC_DIV",
    "TAC_MOVE",
    "TAC_STORE",
    "TAC_LOAD",
    "TAC_PRINT",
    "TAC_READ",
    "TAC_RET",
    "TAC_IFZ",
    "TAC_LABEL",
    "TAC_JUMP",
    "TAC_BEGINFUN",
    "TAC_ENDFUN",
    "TAC_CALL",
    "TAC_ARG",
    "TAC_PARAM",
    "TAC_GTR",
    "TAC_LSS",
    "TAC_EQ",
    "TAC_AND",
    "TAC_OR",
    "TAC_NEG",
    "TAC_NOT"};

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

  const char *typeString =
      (tac->type >= 1 && tac->type < tacTypeCount)
          ? tacTypeStrings[tac->type]
          : "TAC_UNKNOWN";

  cout << typeString
       << ", res=" << (tac->res ? tac->res->text : "null")
       << ", op1=" << (tac->op1 ? tac->op1->text : "null")
       << ", op2=" << (tac->op2 ? tac->op2->text : "null")
       << ")" << endl;
}

TAC *tacJoin(TAC *l1, TAC *l2)
{

  if (!l1)
  {
    return l2;
  }
  if (!l2)
  {
    return l1;
  }

  TAC *tail = l1;
  while (tail->next)
    tail = tail->next;

  tail->next = l2;
  l2->prev = tail;

  return l1;
}

void tacPrintBackwards(TAC *tac)
{
  if (!tac)
    return;

  TAC *first = tac;
  while (first->prev)
    first = first->prev;

  TAC *current = first;
  while (current)
  {
    tacPrint(current);
    current = current->next;
  }
}

TAC *createBinaryOperation(int tacType, Symbol *result,
                           TAC *leftCode, TAC *rightCode)
{
  if (!leftCode || !rightCode)
  {
    cerr << "[ERROR] Missing operands for binary operation." << endl;
    return nullptr;
  }

  if (!result)
  {
    result = makeTemp();
  }

  TAC *opTac = tacCreate(tacType, result,
                         leftCode->res,
                         rightCode->res);

  tacJoin(tacJoin(leftCode, rightCode), opTac);

  return opTac;
}
TAC *generateCode(AST *node)
{
  if (!node)
    return nullptr;

  vector<TAC *> code(node->sons.size(), nullptr);
  for (size_t i = 0; i < node->sons.size(); ++i)
    code[i] = generateCode(node->sons[i]);

  TAC *result = nullptr;

  switch (node->type)
  {
  case AST_PROGRAM:
  case AST_LIST_GLOBAL_VAR_DEC:
  case AST_LIST_GLOBAL_FUNC_DEC:
  case AST_LCMD:
  case AST_BLOCK:
  {
    for (auto &t : code)
      result = tacJoin(result, t);
    break;
  }

  case AST_VAR_DEC:
  {

    Symbol *var = node->sons[1]->symbol;
    Symbol *val = node->sons[2] ? node->sons[2]->symbol : nullptr;

    TAC *move = tacCreate(TAC_MOVE, var, val, nullptr);
    result = tacJoin(result, move);
    break;
  }

  case AST_VEC_DEC:
  {
    for (AST *param = node->sons[3]; param; param = param->sons[1])
    {
      Symbol *val = param->sons[0]->symbol;
      Symbol *vec = node->sons[1]->symbol;
      TAC *mv = tacCreate(TAC_MOVE, vec, val, nullptr);
      result = tacJoin(result, mv);
    }
    break;
  }
  case AST_ASSIGN:
  {
    result = tacJoin(code[0], code[1]);

    Symbol *lhs = node->sons[0]->symbol;
    Symbol *rhs = code[1] ? code[1]->res : nullptr;

    cerr << "[DEBUG] AST_ASSIGN: lhs=" << lhs->text
         << ", rhs=" << (rhs ? rhs->text : "null") << endl;

    if (!rhs)
    {
      cerr << "[ERROR] AST_ASSIGN: Right-hand side is null!" << endl;
      break;
    }

    TAC *moveTac = tacCreate(TAC_MOVE, lhs, rhs, nullptr);
    result = tacJoin(result, moveTac);

    cerr << "[DEBUG] TAC_MOVE created in AST_ASSIGN: "
         << "lhs=" << lhs->text
         << ", rhs=" << (rhs ? rhs->text : "null") << endl;

    break;
  }

  case AST_ASSIGN_VEC:
  {
    result = tacJoin(code[0], code[1]);
    result = tacJoin(result, code[2]);

    Symbol *vec = node->sons[0]->symbol;
    Symbol *idx = code[1] ? code[1]->res : nullptr;
    Symbol *rhs = code[2] ? code[2]->res : nullptr;

    TAC *st = tacCreate(TAC_STORE, vec, idx, rhs);
    result = tacJoin(result, st);
    break;
  }

  case AST_PRINT:
  {
    for (AST *arg = node->sons[0]; arg; arg = arg->sons[1])
    {
      Symbol *s = arg->sons[0] ? arg->sons[0]->symbol : nullptr;
      TAC *p = tacCreate(TAC_PRINT, s, nullptr, nullptr);
      result = tacJoin(result, p);
    }
    break;
  }

  case AST_READ:
  {
    Symbol *id = node->sons[0]->symbol;
    TAC *r = tacCreate(TAC_READ, id, nullptr, nullptr);
    result = tacJoin(result, r);
    break;
  }

  case AST_RETURN:
  {
    result = tacJoin(code[0],
                     tacCreate(TAC_RET,
                               code[0] ? code[0]->res : nullptr,
                               nullptr, nullptr));
    break;
  }

  case AST_IF:
  {
    Symbol *labEnd = makeLabel();

    TAC *ifz = tacCreate(TAC_IFZ, labEnd,
                         code[0] ? code[0]->res : nullptr, nullptr);
    TAC *lab = tacCreate(TAC_LABEL, labEnd, nullptr, nullptr);

    result = tacJoin(code[0], tacJoin(ifz, lab));
    break;
  }

  case AST_IFELSE:
  {
    Symbol *labElse = makeLabel();
    Symbol *labEnd = makeLabel();

    TAC *ifz = tacCreate(TAC_IFZ, labElse,
                         code[0] ? code[0]->res : nullptr, nullptr);
    TAC *jmpEnd = tacCreate(TAC_JUMP, labEnd, nullptr, nullptr);
    TAC *lblElse = tacCreate(TAC_LABEL, labElse, nullptr, nullptr);
    TAC *lblEnd = tacCreate(TAC_LABEL, labEnd, nullptr, nullptr);

    result = tacJoin(code[0],
                     tacJoin(ifz,
                             tacJoin(code[1],
                                     tacJoin(jmpEnd,
                                             tacJoin(lblElse,
                                                     tacJoin(code[2], lblEnd))))));
    break;
  }

  case AST_WHILE:
  {
    Symbol *labBegin = makeLabel();
    Symbol *labEnd = makeLabel();

    TAC *lblBegin = tacCreate(TAC_LABEL, labBegin, nullptr, nullptr);
    TAC *ifz = tacCreate(TAC_IFZ, labEnd,
                         code[0] ? code[0]->res : nullptr, nullptr);
    TAC *jmpBegin = tacCreate(TAC_JUMP, labBegin, nullptr, nullptr);
    TAC *lblEnd = tacCreate(TAC_LABEL, labEnd, nullptr, nullptr);

    result = tacJoin(lblBegin,
                     tacJoin(code[0],
                             tacJoin(ifz,
                                     tacJoin(code[1],
                                             tacJoin(jmpBegin, lblEnd)))));
    break;
  }

  case AST_ADD:
  case AST_SUB:
  case AST_MUL:
  case AST_DIV:
  {
    int binType =
        (node->type == AST_ADD) ? TAC_ADD : (node->type == AST_SUB) ? TAC_SUB
                                        : (node->type == AST_MUL)   ? TAC_MUL
                                                                    : TAC_DIV;

    TAC *binaryOp = createBinaryOperation(binType, nullptr, code[0], code[1]);

    if (binaryOp)
    {
      result = binaryOp;
    }
    else
    {
      cerr << "[ERROR] Failed to generate binary operation TAC." << endl;
    }
    break;
  }

  case AST_FUNC_CALL:
  {
    Symbol *fName = node->sons[0]->symbol;
    TAC *call = tacCreate(TAC_CALL, fName, nullptr, nullptr);
    result = tacJoin(result, call);
    break;
  }

  case AST_FUNC_CALL_PARAMS:
  case AST_NONEMPTY_FUNC_CALL_PARAMS:
  {
    for (auto &t : code)
      result = tacJoin(result, t);
    break;
  }

  case AST_SYMBOL:
  {
    if (node->sons.empty())
    {
      result = tacCreate(TAC_SYMBOL, node->symbol, nullptr, nullptr);
    }
    break;
  }

  default:
  {
    for (auto &t : code)
      result = tacJoin(result, t);
    break;
  }
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
