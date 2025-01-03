#include "semantic.hh"
#include "ast.hh"
#include "mapContainer.h"
#include <iostream>
#include <cstdio>

using namespace std;

int semanticErrors = 0;
AST *ROOT = nullptr;

int verifySemantic(AST *root)
{
  ROOT = root;

  setIdentifierTypes(root);
  setDataTypes(root);
  checkUsageConsistency(root);
  checkUndeclared();

  return semanticErrors;
}

void printAST(AST *node, int depth)
{
  if (!node)
    return;

  for (int i = 0; i < depth; ++i)
    cerr << "  ";

  cerr << "Node type: " << node->type << ", Line: " << node->line << endl;

  for (AST *child : node->sons)
  {
    if (child)
      printAST(child, depth + 1);
  }
}

void setIdentifierTypes(AST *node)
{
  if (!node)
    return;

  switch (node->type)
  {
  case AST_VAR_DEC:
    if (node->sons[1] && node->sons[1]->symbol)
    {
      if (node->sons[1]->symbol->type != SYMBOL_IDENTIFIER)
      {
        cerr << "Semantic error: variable " << node->sons[1]->symbol->text << " already declared" << endl;
        semanticErrors++;
      }
      else
      {
        node->sons[1]->symbol->type = SYMBOL_VAR;
        if (node->sons[0] && node->sons[0]->type == AST_KW_CHAR)
          node->sons[1]->symbol->dataType = DATATYPE_CHAR;
        else if (node->sons[0] && node->sons[0]->type == AST_KW_INT)
          node->sons[1]->symbol->dataType = DATATYPE_INT;
      }
    }
    else
    {
      cerr << "Semantic error: symbol is null for variable declaration" << endl;
      semanticErrors++;
    }
    break;

  case AST_VEC_DEC:
  case AST_VEC_NULL_DEC:
    if (node->sons[1] && node->sons[1]->symbol)
    {
      if (node->sons[1]->symbol->type != SYMBOL_IDENTIFIER)
      {
        cerr << "Semantic error: vector " << node->sons[1]->symbol->text << " already declared" << endl;
        semanticErrors++;
      }
      else
      {
        node->sons[1]->symbol->type = SYMBOL_VECTOR;
        if (node->sons[0] && node->sons[0]->type == AST_KW_CHAR)
          node->sons[1]->symbol->dataType = DATATYPE_CHAR;
        else if (node->sons[0] && node->sons[0]->type == AST_KW_INT)
          node->sons[1]->symbol->dataType = DATATYPE_INT;
      }
    }
    else
    {
      cerr << "Semantic error: symbol is null for vector declaration" << endl;
      semanticErrors++;
    }
    break;

  case AST_FUNCTION_DEC:
    if (node->sons[1] && node->sons[1]->symbol)
    {
      if (node->sons[1]->symbol->type != SYMBOL_IDENTIFIER)
      {
        cerr << "Semantic error: function " << node->sons[1]->symbol->text << " already declared" << endl;
        semanticErrors++;
      }
      else
      {
        node->sons[1]->symbol->type = SYMBOL_FUNC;
        if (node->sons[0] && node->sons[0]->type == AST_KW_CHAR)
          node->sons[1]->symbol->dataType = DATATYPE_CHAR;
        else if (node->sons[0] && node->sons[0]->type == AST_KW_INT)
          node->sons[1]->symbol->dataType = DATATYPE_INT;
      }
    }
    else
    {
      cerr << "Semantic error: symbol is null for function declaration" << endl;
      semanticErrors++;
    }
    break;

  case AST_FUNCTION_DEC_PARAMS:
  case AST_NONEMPTY_FUNCTION_DEC_PARAMS:
    if (node->sons[1] && node->sons[1]->symbol)
    {
      if (node->sons[1]->symbol->type != SYMBOL_IDENTIFIER)
      {
        cerr << "Semantic error: parameter " << node->sons[1]->symbol->text << " already declared" << endl;
        semanticErrors++;
      }
      else
      {
        node->sons[1]->symbol->type = SYMBOL_VAR;
        if (node->sons[0] && node->sons[0]->type == AST_KW_CHAR)
          node->sons[1]->symbol->dataType = DATATYPE_CHAR;
        else if (node->sons[0] && node->sons[0]->type == AST_KW_INT)
          node->sons[1]->symbol->dataType = DATATYPE_INT;
      }
    }
    else
    {
      cerr << "Semantic error: function parameter node or symbol is null" << endl;
      semanticErrors++;
    }
    break;

  default:
    break;
  }

  for (auto &child : node->sons)
  {
    if (child)
      setIdentifierTypes(child);
  }
}

void checkUndeclared()
{
  for (const auto &entry : symbolTable)
  {
    const string &name = entry.first;
    const Symbol &symbol = entry.second;

    if (symbol.type == SYMBOL_IDENTIFIER)
    {
      cerr << "Semantic error : variable " << name << " not declared" << endl;
      semanticErrors++;
    }
  }
}

void setDataTypes(AST *node)
{
  if (!node)
    return;

  switch (node->type)
  {
  case AST_SYMBOL:
    if (node->symbol)
    {
      node->dataType = node->symbol->dataType;
      cerr << "DEBUG: Set dataType for SYMBOL node at line " << node->line
           << " to " << node->dataType << endl;
    }
    break;

  case AST_ADD:
  case AST_SUB:
  case AST_MUL:
  case AST_DIV:
    setDataTypes(node->sons[0]);
    setDataTypes(node->sons[1]);
    if (isInteger(node->sons[0]->dataType) && isInteger(node->sons[1]->dataType))
    {
      node->dataType = DATATYPE_INT;
    }
    else
    {
      cerr << "Semantic error: Invalid operand types for arithmetic operation at line " << node->line << endl;
      semanticErrors++;
    }
    break;

  case AST_AND:
  case AST_OR:
    setDataTypes(node->sons[0]);
    setDataTypes(node->sons[1]);
    if (node->sons[0]->dataType == DATATYPE_BOOL && node->sons[1]->dataType == DATATYPE_BOOL)
    {
      node->dataType = DATATYPE_BOOL;
    }
    else
    {
      cerr << "Semantic error: Invalid operand types for logical operation at line " << node->line << endl;
      semanticErrors++;
    }
    break;

  case AST_EQ:
  case AST_GTR:
  case AST_LSS:
    setDataTypes(node->sons[0]);
    setDataTypes(node->sons[1]);
    if (isInteger(node->sons[0]->dataType) && isInteger(node->sons[1]->dataType))
    {
      node->dataType = DATATYPE_BOOL;
    }
    else
    {
      cerr << "Semantic error: Invalid operand types for relational operation at line " << node->line << endl;
      semanticErrors++;
    }
    break;

  case AST_NEG:
    setDataTypes(node->sons[0]);
    if (node->sons[0]->dataType == DATATYPE_BOOL)
    {
      node->dataType = DATATYPE_BOOL;
    }
    else
    {
      cerr << "Semantic error: Invalid operand type for negation at line " << node->line << endl;
      semanticErrors++;
    }
    break;

  case AST_VEC_DEC:
    setDataTypes(node->sons[0]);
    setDataTypes(node->sons[2]);
    validateVectorInitialization(node);
    break;

  default:
    for (auto &child : node->sons)
    {
      if (child)
        setDataTypes(child);
    }
    break;
  }
}

void checkTypeCompatibility(AST *node)
{
  if (!node || node->type != AST_ASSIGN)
    return;

  if (node->sons[0] && node->sons[1])
  {
    int leftType = node->sons[0]->symbol ? node->sons[0]->symbol->dataType : -1;
    int rightType = inferExpressionType(node->sons[1]);

    if (leftType != -1 && rightType != -1)
    {
      if ((isInteger(leftType) && isInteger(rightType)) || leftType == rightType)
        return;

      cerr << "Semantic error: Type mismatch in assignment at line " << node->line << ". Left: " << leftType << ", Right: " << rightType << endl;
      semanticErrors++;
    }
  }
}

int inferExpressionType(AST *node)
{
  if (!node)
    return -1;

  switch (node->type)
  {
  case AST_ADD:
  case AST_SUB:
  case AST_MUL:
  case AST_DIV:
    if (node->sons[0] && node->sons[1])
    {
      int leftType = inferExpressionType(node->sons[0]);
      int rightType = inferExpressionType(node->sons[1]);
      if (isInteger(leftType) && isInteger(rightType))
        return DATATYPE_INT;
    }
    break;

  case AST_AND:
  case AST_OR:
    if (node->sons[0] && node->sons[1])
    {
      int leftType = inferExpressionType(node->sons[0]);
      int rightType = inferExpressionType(node->sons[1]);
      if (leftType == DATATYPE_BOOL && rightType == DATATYPE_BOOL)
        return DATATYPE_BOOL;
    }
    break;

  case AST_EQ:
  case AST_GTR:
  case AST_LSS:
    if (node->sons[0] && node->sons[1])
    {
      int leftType = inferExpressionType(node->sons[0]);
      int rightType = inferExpressionType(node->sons[1]);
      if (isInteger(leftType) && isInteger(rightType))
        return DATATYPE_BOOL;
    }
    break;

  case AST_NEG:
    if (node->sons[0])
    {
      int operandType = inferExpressionType(node->sons[0]);
      if (operandType == DATATYPE_BOOL)
        return DATATYPE_BOOL;
    }
    break;

  case AST_SYMBOL:
    return node->symbol ? node->symbol->dataType : -1;

  default:
    break;
  }

  return -1;
}

void checkUsageConsistency(AST *node)
{
  if (!node)
    return;

  switch (node->type)
  {
  case AST_ASSIGN:
    if (node->sons[0] && node->sons[0]->symbol)
    {
      if (node->sons[0]->symbol->type != SYMBOL_VAR)
      {
        cerr << "Semantic error: Identifier " << node->sons[0]->symbol->text << " must be a variable at line " << node->line << endl;
        semanticErrors++;
      }
    }
    checkTypeCompatibility(node);
    break;

  case AST_ASSIGN_VEC:
    if (node->sons[0] && node->sons[0]->symbol)
    {
      if (node->sons[0]->symbol->type != SYMBOL_VECTOR)
      {
        cerr << "Semantic error: Identifier " << node->sons[0]->symbol->text << " must be a vector at line " << node->line << endl;
        semanticErrors++;
      }

      if (node->sons[1] && node->sons[1]->symbol)
      {
        int indexDataType = node->sons[1]->symbol->dataType;
        if (!isInteger(indexDataType))
        {
          cerr << "Semantic error: Index for vector " << node->sons[0]->symbol->text << " must be an integer at line " << node->line << endl;
          semanticErrors++;
        }
      }
    }
    break;

  case AST_FUNC_CALL:
    if (node->sons[0] && node->sons[0]->symbol)
    {
      if (node->sons[0]->symbol->type != SYMBOL_FUNC)
      {
        cerr << "Semantic error: Identifier " << node->sons[0]->symbol->text << " must be a function at line " << node->line << endl;
        semanticErrors++;
      }
      else
      {
        validateFunctionArguments(node);
      }
    }
    break;

  default:
    break;
  }

  for (auto &child : node->sons)
  {
    if (child)
      checkUsageConsistency(child);
  }
}

int isInteger(int datatype)
{
  return (datatype == DATATYPE_INT || datatype == DATATYPE_CHAR);
}

void validateVectorInitialization(AST *node)
{
  if (!node->sons[2] || !node->sons[2]->symbol)
  {
    cerr << "Semantic error: Missing or invalid size definition for vector at line " << node->line << endl;
    semanticErrors++;
    return;
  }

  if (node->sons[2]->symbol->type != SYMBOL_LIT_INT)
  {
    cerr << "Semantic error: Vector size must be an integer literal, found '"
         << node->sons[2]->symbol->text << "' at line " << node->line << endl;
    semanticErrors++;
    return;
  }

  int declaredSize = 0;
  string sizeText = node->sons[2]->symbol->text;

  if (!sizeText.empty() && sizeText[0] == '#')
    sizeText = sizeText.substr(1);

  try
  {
    declaredSize = stoi(sizeText);
  }
  catch (const exception &e)
  {
    cerr << "Semantic error: Invalid vector size '" << node->sons[2]->symbol->text
         << "' at line " << node->line << endl;
    semanticErrors++;
    return;
  }

  int actualSize = countVectorElements(node->sons[3]);

  if (declaredSize != actualSize)
  {
    cerr << "Semantic error: Mismatch in vector initialization size at line " << node->line
         << ". Declared: " << declaredSize << ", Actual: " << actualSize << endl;
    semanticErrors++;
  }

  if (!validateVectorTypes(node->sons[0]->dataType, node->sons[3]))
  {
    cerr << "Semantic error: Incompatible types in vector initialization at line "
         << node->line << endl;
    semanticErrors++;
  }
}

int countVectorElements(AST *node)
{
  if (!node)
    return 0;

  if (node->type == AST_VEC_PARAMS)
    return 1 + countVectorElements(node->sons[1]);

  return 1;
}

bool validateVectorTypes(int vectorType, AST *elements)
{
  if (!elements)
    return true;

  if (elements->type == AST_VEC_PARAMS)
  {
    return validateVectorTypes(vectorType, elements->sons[0]) && validateVectorTypes(vectorType, elements->sons[1]);
  }

  return elements->dataType == vectorType;
}

AST *findFunctionDeclaration(Symbol *symbol, AST *node)
{
  if (!node)
    node = ROOT;

  if (node->type == AST_FUNCTION_DEC && node->sons[1]->symbol == symbol)
  {
    return node;
  }

  for (AST *child : node->sons)
  {
    if (child)
    {
      AST *result = findFunctionDeclaration(symbol, child);
      if (result)
        return result;
    }
  }

  return nullptr;
}

void validateFunctionArguments(AST *node)
{
  if (!node->sons[0] || !node->sons[0]->symbol)
  {
    cerr << "Semantic error: Function identifier missing at line " << node->line << endl;
    semanticErrors++;
    return;
  }

  Symbol *functionSymbol = node->sons[0]->symbol;

  if (functionSymbol->type != SYMBOL_FUNC)
  {
    cerr << "Semantic error: Identifier '" << functionSymbol->text << "' is not a function at line " << node->line << endl;
    semanticErrors++;
    return;
  }

  AST *functionDeclaration = findFunctionDeclaration(functionSymbol);
  if (!functionDeclaration)
  {
    cerr << "Semantic error: Function '" << functionSymbol->text << "' not declared at line " << node->line << endl;
    semanticErrors++;
    return;
  }

  AST *declaredParams = functionDeclaration->sons[2];
  AST *passedArgs = node->sons[1];

  cerr << "DEBUG: Declared parameters for function '" << functionSymbol->text << "':" << endl;
  printAST(declaredParams, 1);

  cerr << "DEBUG: Passed arguments for function '" << functionSymbol->text << "' at line " << node->line << ":" << endl;
  printAST(passedArgs, 1);

  int declaredCount = countFunctionParameters(declaredParams);
  int passedCount = countFunctionParameters(passedArgs);

  cerr << "DEBUG: Declared count: " << declaredCount << ", Passed count: " << passedCount << endl;

  if (declaredCount != passedCount)
  {
    cerr << "Semantic error: Mismatch in number of arguments for function '" << functionSymbol->text
         << "' at line " << node->line << ". Declared: " << declaredCount << ", Passed: " << passedCount << endl;
    semanticErrors++;
    return;
  }

  if (!validateParameterTypes(declaredParams, passedArgs))
  {
    cerr << "Semantic error: Type mismatch in arguments for function '" << functionSymbol->text
         << "' at line " << node->line << endl;
    semanticErrors++;
  }
}

int countFunctionParameters(AST *node)
{
  if (!node)
    return 0;

  cerr << "DEBUG: Counting parameters. Node type: " << node->type << ", Line: " << node->line << endl;

  if (node->type == AST_NONEMPTY_FUNC_CALL_PARAMS)
  {
    int count = 1 + countFunctionParameters(node->sons[1]);
    cerr << "DEBUG: Parameters counted so far: " << count << endl;
    return count;
  }

  if (node->type == AST_NONEMPTY_FUNCTION_DEC_PARAMS)
  {
    int count = 1 + countFunctionParameters(node->sons[2]);
    cerr << "DEBUG: Parameters counted so far: " << count << endl;
    return count;
  }

  if (node->type == AST_FUNC_CALL_PARAMS || node->type == AST_FUNCTION_DEC_PARAMS)
  {
    int count = node->sons[0] ? 1 : 0;
    cerr << "DEBUG: Single parameter node detected. Count: " << count << endl;
    return count;
  }

  cerr << "DEBUG: No parameters detected in current node." << endl;
  return 0;
}

bool validateParameterTypes(AST *declared, AST *passed)
{
  if (!declared || !passed)
    return true;

  if (declared->dataType == -1 || passed->dataType == -1)
  {
    cerr << "DEBUG: Type not set for either declared or passed parameter." << endl;
    return false;
  }

  if ((declared->type == AST_NONEMPTY_FUNCTION_DEC_PARAMS || declared->type == AST_FUNCTION_DEC_PARAMS) &&
      (passed->type == AST_NONEMPTY_FUNC_CALL_PARAMS || passed->type == AST_FUNC_CALL_PARAMS))
  {
    bool result = validateParameterTypes(declared->sons[1], passed->sons[1]) &&
                  (isInteger(declared->sons[0]->dataType) && isInteger(passed->sons[0]->dataType));
    cerr << "DEBUG: Recursive parameter type comparison result: " << result << endl;
    return result;
  }

  bool result = (isInteger(declared->dataType) && isInteger(passed->dataType)) ||
                (declared->dataType == passed->dataType);

  cerr << "DEBUG: Final parameter type comparison result: " << result << endl;
  return result;
}
