// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include <iostream>
#include <vector>
#include "tacs.hh"
#include "mapContainer.h"

using namespace std;
const char *tacTypeStrings[] = {
    "TAC_UNKNOWN",
    "TAC_SYMBOL",   // Representa símbolos
    "TAC_ADD",      // Soma
    "TAC_SUB",      // Subtração
    "TAC_MUL",      // Multiplicação
    "TAC_DIV",      // Divisão
    "TAC_MOVE",     // Movimentação (Atribuição)
    "TAC_STORE",    // Armazenamento em vetor/matriz
    "TAC_LOAD",     // Carregamento de vetor/matriz
    "TAC_PRINT",    // Imprimir
    "TAC_READ",     // Ler entrada
    "TAC_RET",      // Retorno de função
    "TAC_IFZ",      // Salto condicional (se zero)
    "TAC_LABEL",    // Rótulo
    "TAC_JUMP",     // Salto incondicional
    "TAC_BEGINFUN", // Início de função
    "TAC_ENDFUN",   // Fim de função
    "TAC_CALL",     // Chamada de função
    "TAC_ARG",      // Argumento de função
    "TAC_PARAM",    // Parâmetro de função
    "TAC_GTR",      // Comparação maior
    "TAC_LSS",      // Comparação menor
    "TAC_EQ",       // Comparação de igualdade
    "TAC_AND",      // Operação lógica AND
    "TAC_OR",       // Operação lógica OR
    "TAC_NEG",      // Negação lógica
    "TAC_NOT"       // Operação NOT
};

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

  std::cerr << "[DEBUG] Created TAC: " << tacTypeStrings[type] << ", res="
            << (res ? res->text : "null") << std::endl;

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
  std::cerr << "[DEBUG] tacJoin: Joining l1 ("
            << (l1 ? l1->res->text : "null")
            << ") with l2 ("
            << (l2 ? l2->res->text : "null")
            << ")" << std::endl;

  if (!l1)
  {
    std::cerr << "[DEBUG] tacJoin: Returning l2: "
              << (l2 ? l2->res->text : "null")
              << std::endl;
    return l2;
  }
  if (!l2)
  {
    std::cerr << "[DEBUG] tacJoin: Returning l1: "
              << (l1 ? l1->res->text : "null")
              << std::endl;
    return l1;
  }

  // Encontrar o último elemento de l1
  TAC *tail = l1;
  while (tail->next)
  {
    tail = tail->next;
  }

  // Conectar l1 ao início de l2
  tail->next = l2;
  l2->prev = tail;

  std::cerr << "[DEBUG] tacJoin: Joined lists. New tail: "
            << tail->res->text
            << ", New head of l2: "
            << l2->res->text
            << std::endl;

  return l1; // Retornar o início da lista unida
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
    // std::cerr << "[DEBUG] Printing TAC: Type=" << tacTypeStrings[current->type]
    //           << ", res=" << (current->res ? current->res->text : "null")
    //           << ", prev=" << (current->prev ? current->prev->res->text : "null")
    //           << ", next=" << (current->next ? current->next->res->text : "null") << std::endl;
    tacPrint(current);
    current = current->next;
  }
}

TAC *generateCode(AST *node)
{
  if (!node)
    return nullptr;

  TAC *result = nullptr;
  std::vector<TAC *> code(node->sons.size(), nullptr);

  for (size_t i = 0; i < node->sons.size(); ++i)
  {
    code[i] = generateCode(node->sons[i]);
  }

  switch (node->type)
  {
  case AST_PROGRAM:
  case AST_LIST_GLOBAL_VAR_DEC:
  case AST_LIST_GLOBAL_FUNC_DEC:
  case AST_LCMD:
  case AST_BLOCK:
    for (auto &tac : code)
    {
      result = tacJoin(result, tac);
    }
    break;

  case AST_VAR_DEC:
    std::cerr << "[DEBUG] Processing AST_VAR_DEC for variable: "
              << (node->sons[1] ? node->sons[1]->symbol->text : "null")
              << " with value: "
              << (node->sons[2] ? node->sons[2]->symbol->text : "null")
              << std::endl;

    result = tacCreate(TAC_MOVE, node->sons[1]->symbol,
                       node->sons[2] ? node->sons[2]->symbol : nullptr,
                       nullptr);
    break;

  case AST_VEC_DEC:
    for (AST *param = node->sons[3]; param; param = param->sons[1])
    {
      result = tacJoin(result, tacCreate(TAC_MOVE, node->sons[1]->symbol, param->sons[0]->symbol, nullptr));
    }
    break;

  case AST_ASSIGN:
  {
    // Junta lado esquerdo e direito
    result = tacJoin(code[0], code[1]);

    // Atribui para o identificador, usando como op1 o ->res do lado direito
    Symbol *rhs = code[1] ? code[1]->res : nullptr;
    TAC *moveTac = tacCreate(TAC_MOVE, node->sons[0]->symbol, rhs, nullptr);

    result = tacJoin(result, moveTac);
    break;
  }

  case AST_ASSIGN_VEC:
  {
    // code[0]: nó do identificador do vetor
    // code[1]: índice (expr)
    // code[2]: expressão da direita
    result = tacJoin(code[0], code[1]);
    result = tacJoin(result, code[2]);

    Symbol *index = code[1] ? code[1]->res : nullptr;
    Symbol *rhs = code[2] ? code[2]->res : nullptr;
    Symbol *vecName = node->sons[0]->symbol;
    TAC *storeTac = tacCreate(TAC_STORE, vecName, index, rhs);

    result = tacJoin(result, storeTac);
    break;
  }

  case AST_PRINT:
    for (AST *arg = node->sons[0]; arg; arg = arg->sons[1])
    {
      result = tacJoin(result, tacCreate(TAC_PRINT, arg->sons[0]->symbol, nullptr, nullptr));
    }
    break;

  case AST_READ:
    result = tacCreate(TAC_READ, node->sons[0]->symbol, nullptr, nullptr);
    break;

  case AST_RETURN:
    result = tacJoin(code[0], tacCreate(TAC_RET, code[0] ? code[0]->res : nullptr, nullptr, nullptr));
    break;

  case AST_IF:
  {
    Symbol *labelEnd = makeLabel();
    result = tacJoin(code[0],
                     tacJoin(tacCreate(TAC_IFZ, labelEnd, code[0] ? code[0]->res : nullptr, nullptr),
                             tacCreate(TAC_LABEL, labelEnd, nullptr, nullptr)));
    break;
  }

  case AST_IFELSE:
  {
    Symbol *labelElse = makeLabel();
    Symbol *labelEnd = makeLabel();
    result = tacJoin(code[0],
                     tacJoin(tacCreate(TAC_IFZ, labelElse, code[0] ? code[0]->res : nullptr, nullptr),
                             tacJoin(code[1],
                                     tacJoin(tacCreate(TAC_JUMP, labelEnd, nullptr, nullptr),
                                             tacJoin(tacCreate(TAC_LABEL, labelElse, nullptr, nullptr),
                                                     tacJoin(code[2],
                                                             tacCreate(TAC_LABEL, labelEnd, nullptr, nullptr)))))));
    break;
  }

  case AST_WHILE:
  {
    Symbol *labelBegin = makeLabel();
    Symbol *labelEnd = makeLabel();
    result = tacJoin(tacCreate(TAC_LABEL, labelBegin, nullptr, nullptr),
                     tacJoin(code[0],
                             tacJoin(tacCreate(TAC_IFZ, labelEnd, code[0] ? code[0]->res : nullptr, nullptr),
                                     tacJoin(code[1],
                                             tacJoin(tacCreate(TAC_JUMP, labelBegin, nullptr, nullptr),
                                                     tacCreate(TAC_LABEL, labelEnd, nullptr, nullptr))))));
    break;
  }

  case AST_ADD:
  case AST_SUB:
  case AST_MUL:
  case AST_DIV:
  {
    std::cerr << "[DEBUG] Processing binary operation for node type: "
              << ((node->type == AST_ADD) ? "ADD" : (node->type == AST_SUB) ? "SUB"
                                                : (node->type == AST_MUL)   ? "MUL"
                                                                            : "DIV")
              << std::endl;

    // Criar o TAC para a operação binária
    TAC *binaryOp = createBinaryOperation(
        (node->type == AST_ADD) ? TAC_ADD : (node->type == AST_SUB) ? TAC_SUB
                                        : (node->type == AST_MUL)   ? TAC_MUL
                                                                    : TAC_DIV,
        nullptr, code[0], code[1]);

    // Verificar se a operação foi bem-sucedida
    if (binaryOp)
    {
      std::cerr << "[DEBUG] Binary operation TAC created: " << binaryOp->res->text << std::endl;
      result = tacJoin(result, binaryOp); // Combine o TAC binário com o resultado
    }
    else
    {
      std::cerr << "[ERROR] Failed to create TAC for binary operation." << std::endl;
    }

    break;
  }

  case AST_FUNC_CALL:
    result = tacCreate(TAC_CALL, node->sons[0]->symbol, nullptr, nullptr);
    break;

  case AST_FUNC_CALL_PARAMS:
  case AST_NONEMPTY_FUNC_CALL_PARAMS:
    for (auto &tac : code)
    {
      result = tacJoin(result, tac);
    }
    break;

  case AST_SYMBOL:
    result = tacCreate(TAC_SYMBOL, node->symbol, nullptr, nullptr);
    break;

  default:
    for (auto &tac : code)
    {
      result = tacJoin(result, tac);
    }
    std::cerr << "[DEBUG] Combined TACs in default case: "
              << (result ? result->res->text : "null") << std::endl;
    break;
  }

  // std::cerr << "[DEBUG] Generating TAC for node type: " << node->type << std::endl;

  std::cerr << "[DEBUG] generateCode: Returning TAC with head="
            << (result ? result->res->text : "null") << std::endl;
  return result;
}

TAC *createBinaryOperation(int tacType, Symbol *result, TAC *leftCode, TAC *rightCode)
{
  if (!leftCode || !rightCode)
  {
    std::cerr << "[ERROR] Missing operands for binary operation." << std::endl;
    return nullptr;
  }

  // Criar temporário para o resultado, se necessário
  if (!result)
  {
    result = makeTemp();
    std::cerr << "[DEBUG] Created temporary symbol for result: " << result->text << std::endl;
  }

  // Criar o TAC da operação binária
  TAC *operationTac = tacCreate(tacType, result, leftCode->res, rightCode->res);

  std::cerr << "[DEBUG] Created binary operation TAC: " << tacTypeStrings[tacType]
            << ", result=" << result->text << std::endl;

  // Combinar TACs
  return tacJoin(tacJoin(leftCode, rightCode), operationTac);
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
