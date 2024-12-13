// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include "ast.hh"
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

string astTypeName[] = {
    "AST_PROGRAM",
    "AST_LIST_GLOBAL_VAR_DEC",
    "AST_LIST_GLOBAL_FUNC_DEC",
    "AST_VAR_DEC",
    "AST_VEC_NULL_DEC",
    "AST_VEC_DEC",
    "AST_VEC_PARAMS",
    "AST_FUNCTION_DEC",
    "AST_FUNCTION_DEC_PARAMS",
    "AST_NONEMPTY_FUNCTION_DEC_PARAMS",
    "AST_BLOCK",
    "AST_LCMD",
    "AST_ASSIGN",
    "AST_ASSIGN_VEC",
    "AST_PRINT",
    "AST_READ",
    "AST_RETURN",
    "AST_IF",
    "AST_IFELSE",
    "AST_WHILE",
    "AST_NONEMPTY_LIST_PRINT_ARGS_STRING",
    "AST_NONEMPTY_LIST_PRINT_ARGS_EXPR",
    "AST_LIST_PRINT_ARGS_STRING",
    "AST_LIST_PRINT_ARGS_EXPR",
    "AST_PARENS",
    "AST_VEC_CALL",
    "AST_ADD",
    "AST_SUB",
    "AST_MUL",
    "AST_DIV",
    "AST_GTR",
    "AST_LSS",
    "AST_AND",
    "AST_OR",
    "AST_EQ",
    "AST_NEG",
    "AST_FUNC_CALL",
    "AST_FUNC_CALL_PARAMS",
    "AST_NONEMPTY_FUNC_CALL_PARAMS",
    "AST_SYMBOL",
    "AST_KW_CHAR",
    "AST_KW_INT"};

void astPrint(AST *node, int level)
{
  if (!node)
    return;

  for (int i = 0; i < level; i++)
    fprintf(stderr, "\t");

  fprintf(stderr, "AST(%s, %s)\n",
          astTypeName[node->type].c_str(),
          node->symbol ? node->symbol->text.c_str() : "");

  for (auto s : node->sons)
    astPrint(s, level + 1);
}

void astPrintCode(AST *node, std::ostream &output)
{
  if (!node)
    return;

  switch (node->type)
  {
  case AST_PROGRAM:
  case AST_LIST_GLOBAL_VAR_DEC:
  case AST_LIST_GLOBAL_FUNC_DEC:
  case AST_LCMD:
    for (auto s : node->sons)
      if (s)
        astPrintCode(s, output);
    break;

  case AST_VAR_DEC:
    astPrintCode(node->sons[0], output);
    output << " ";
    astPrintCode(node->sons[1], output);
    output << " = ";
    astPrintCode(node->sons[2], output);
    output << ";\n";
    break;

  case AST_VEC_NULL_DEC:
    astPrintCode(node->sons[0], output);
    output << " ";
    astPrintCode(node->sons[1], output);
    output << "[";
    astPrintCode(node->sons[2], output);
    output << "];\n";
    break;

  case AST_VEC_DEC:
    astPrintCode(node->sons[0], output);
    output << " ";
    astPrintCode(node->sons[1], output);
    output << "[";
    astPrintCode(node->sons[2], output);
    output << "]";
    if (node->sons[3])
    {
      output << " = ";
      astPrintCode(node->sons[3], output);
    }
    output << ";\n";
    break;

  case AST_VEC_PARAMS:
    astPrintCode(node->sons[0], output);
    if (node->sons[1])
    {
      output << " ";
      astPrintCode(node->sons[1], output);
    }
    break;

  case AST_FUNCTION_DEC:
    astPrintCode(node->sons[0], output);
    output << " ";
    astPrintCode(node->sons[1], output);
    output << " (";
    if (node->sons[2])
      astPrintCode(node->sons[2], output);
    output << ")\n";
    astPrintCode(node->sons[3], output);
    break;

  case AST_FUNCTION_DEC_PARAMS:
  case AST_NONEMPTY_FUNCTION_DEC_PARAMS:
    astPrintCode(node->sons[0], output);
    output << " ";
    astPrintCode(node->sons[1], output);
    if (node->sons[2])
    {
      output << ", ";
      astPrintCode(node->sons[2], output);
    }
    break;

  case AST_ASSIGN:
    astPrintCode(node->sons[0], output);
    output << " = ";
    astPrintCode(node->sons[1], output);
    output << ";\n";
    break;

  case AST_ASSIGN_VEC:
    astPrintCode(node->sons[0], output);
    output << "[";
    astPrintCode(node->sons[1], output);
    output << "] = ";
    astPrintCode(node->sons[2], output);
    output << ";\n";
    break;

  case AST_PRINT:
    output << "print ";
    astPrintCode(node->sons[0], output);
    output << ";\n";
    break;

  case AST_READ:
    output << "read ";
    astPrintCode(node->sons[0], output);
    output << ";\n";
    break;

  case AST_RETURN:
    output << "return ";
    astPrintCode(node->sons[0], output);
    output << ";\n";
    break;

  case AST_IF:
    output << "if (";
    astPrintCode(node->sons[0], output);
    output << ") then\n";
    astPrintCode(node->sons[1], output);
    break;

  case AST_IFELSE:
    output << "if (";
    astPrintCode(node->sons[0], output);
    output << ") then\n";
    astPrintCode(node->sons[1], output);
    output << " else ";
    astPrintCode(node->sons[2], output);
    break;

  case AST_WHILE:
    output << "while (";
    astPrintCode(node->sons[0], output);
    output << ") \n";
    astPrintCode(node->sons[1], output);
    break;

  case AST_ADD:
    astPrintCode(node->sons[0], output);
    output << " + ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_SUB:
    astPrintCode(node->sons[0], output);
    output << " - ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_MUL:
    astPrintCode(node->sons[0], output);
    output << " * ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_DIV:
    astPrintCode(node->sons[0], output);
    output << " / ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_LSS:
    astPrintCode(node->sons[0], output);
    output << " < ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_GTR:
    astPrintCode(node->sons[0], output);
    output << " > ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_EQ:
    astPrintCode(node->sons[0], output);
    output << " = ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_AND:
    astPrintCode(node->sons[0], output);
    output << " & ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_OR:
    astPrintCode(node->sons[0], output);
    output << " | ";
    astPrintCode(node->sons[1], output);
    break;

  case AST_NEG:
    output << "~ ";
    astPrintCode(node->sons[0], output);
    break;

  case AST_FUNC_CALL:
    astPrintCode(node->sons[0], output);
    output << "(";
    if (node->sons[1])
      astPrintCode(node->sons[1], output);
    output << ")";
    break;

  case AST_FUNC_CALL_PARAMS:
  case AST_NONEMPTY_FUNC_CALL_PARAMS:
    astPrintCode(node->sons[0], output);
    if (node->sons[1])
    {
      output << ", ";
      astPrintCode(node->sons[1], output);
    }
    break;

  case AST_VEC_CALL:
    astPrintCode(node->sons[0], output);
    output << "[";
    astPrintCode(node->sons[1], output);
    output << "]";
    break;

  case AST_NONEMPTY_LIST_PRINT_ARGS_EXPR:
  case AST_LIST_PRINT_ARGS_EXPR:
  case AST_NONEMPTY_LIST_PRINT_ARGS_STRING:
  case AST_LIST_PRINT_ARGS_STRING:
    astPrintCode(node->sons[0], output);
    if (node->sons[1])
    {
      output << " ";
      astPrintCode(node->sons[1], output);
    }
    break;

  case AST_SYMBOL:
    if (node->symbol && !node->symbol->text.empty())
      output << node->symbol->text;
    else
      output << "INVALID_SYMBOL";
    break;

  case AST_KW_CHAR:
    output << "char";
    break;

  case AST_KW_INT:
    output << "int";
    break;

  case AST_BLOCK:
    output << "{\n";
    for (auto s : node->sons)
      if (s)
        astPrintCode(s, output);
    output << "}";
    break;

  default:
    output << "UNKNOWN_AST_TYPE(" << astTypeName[node->type] << ")";
    break;
  }
}
