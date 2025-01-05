// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
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
  validateUndeclaredIdentifiers();

  return semanticErrors;
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
        node->sons[1]->symbol->dataType = (node->sons[0] && node->sons[0]->type == AST_KW_CHAR)
                                              ? DATATYPE_CHAR
                                              : DATATYPE_INT;
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
        node->sons[1]->symbol->dataType = (node->sons[0] && node->sons[0]->type == AST_KW_CHAR)
                                              ? DATATYPE_CHAR
                                              : DATATYPE_INT;
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
        cerr << "Semantic error: function " << node->sons[1]->symbol->text << " already declared at line " << node->line << endl;
        semanticErrors++;
      }
      else
      {
        node->sons[1]->symbol->type = SYMBOL_FUNC;
        node->sons[1]->symbol->dataType = (node->sons[0] && node->sons[0]->type == AST_KW_CHAR)
                                              ? DATATYPE_CHAR
                                              : DATATYPE_INT;

        node->dataType = node->sons[1]->symbol->dataType;
      }
    }
    else
    {
      cerr << "Semantic error: symbol is null for function declaration at line " << node->line << endl;
      semanticErrors++;
    }
    break;

  case AST_FUNCTION_DEC_PARAMS:
  case AST_NONEMPTY_FUNCTION_DEC_PARAMS:
    if (node->sons[1] && node->sons[1]->symbol)
    {
      if (node->sons[1]->symbol->type != SYMBOL_IDENTIFIER)
      {
        cerr << "Semantic error: parameter " << node->sons[1]->symbol->text
             << " already declared at line " << node->line << endl;
        semanticErrors++;
      }
      else
      {
        node->sons[1]->symbol->type = SYMBOL_PARAM;
        node->sons[1]->symbol->dataType = (node->sons[0] && node->sons[0]->type == AST_KW_CHAR)
                                              ? DATATYPE_CHAR
                                              : DATATYPE_INT;
      }
    }
    else
    {
      cerr << "Semantic error: symbol is null for parameter declaration at line " << node->line << endl;
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
    }
    break;

  case AST_ADD:
  case AST_SUB:
  case AST_MUL:
  case AST_DIV:
    setDataTypes(node->sons[0]);
    setDataTypes(node->sons[1]);
    if (areTypesCompatible(node->sons[0]->dataType, node->sons[1]->dataType))
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

  case AST_PARENS:
    setDataTypes(node->sons[0]);
    node->dataType = node->sons[0]->dataType;
    break;

  case AST_VEC_DEC:
    setDataTypes(node->sons[0]);
    setDataTypes(node->sons[2]);
    validateVectorInitialization(node);
    break;

  case AST_FUNCTION_DEC:
    if (node->sons[2])
    {
      setDataTypes(node->sons[2]);
    }
    setDataTypes(node->sons[0]);
    setDataTypes(node->sons[3]);
    break;

  case AST_FUNC_CALL:
    setDataTypes(node->sons[0]);
    setDataTypes(node->sons[1]);
    if (node->sons[0])
    {
      node->dataType = node->sons[0]->dataType;
    }
    break;

  case AST_RETURN:
    if (node->sons[0])
    {
      setDataTypes(node->sons[0]);
      node->dataType = node->sons[0]->dataType;
    }
    else
    {
      node->dataType = -1;
    }
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

void validateAssignmentTypes(AST *node)
{
  if (!node || node->type != AST_ASSIGN)
    return;

  if (node->sons[0] && node->sons[1])
  {
    int leftType = node->sons[0]->symbol ? node->sons[0]->symbol->dataType : -1;
    int rightType = inferExpressionType(node->sons[1]);

    if (leftType != -1 && rightType != -1)
    {
      if (areTypesCompatible(leftType, rightType))
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

  if (isArithmeticOperation(node->type))
  {
    if (node->sons[0] && node->sons[1])
    {
      int leftType = inferExpressionType(node->sons[0]);
      int rightType = inferExpressionType(node->sons[1]);
      if (areTypesCompatible(leftType, rightType))
        return DATATYPE_INT;
    }
  }
  else if (isRelationalOperation(node->type))
  {
    if (node->sons[0] && node->sons[1])
    {
      int leftType = inferExpressionType(node->sons[0]);
      int rightType = inferExpressionType(node->sons[1]);
      if (isInteger(leftType) && isInteger(rightType))
        return DATATYPE_BOOL;
    }
  }
  else if (isLogicalOperation(node->type))
  {
    if (node->sons[0] && node->sons[1])
    {
      int leftType = inferExpressionType(node->sons[0]);
      int rightType = inferExpressionType(node->sons[1]);
      if (leftType == DATATYPE_BOOL && rightType == DATATYPE_BOOL)
        return DATATYPE_BOOL;
    }
  }
  else if (node->type == AST_NEG)
  {
    if (node->sons[0])
    {
      int operandType = inferExpressionType(node->sons[0]);
      if (operandType == DATATYPE_BOOL)
        return DATATYPE_BOOL;
    }
  }
  else if (node->type == AST_SYMBOL)
  {
    return node->symbol ? node->symbol->dataType : -1;
  }

  return -1;
}

bool isArithmeticOperation(int nodeType)
{
  return nodeType == AST_ADD || nodeType == AST_SUB ||
         nodeType == AST_MUL || nodeType == AST_DIV;
}

bool isRelationalOperation(int nodeType)
{
  return nodeType == AST_EQ || nodeType == AST_GTR ||
         nodeType == AST_LSS;
}

bool isLogicalOperation(int nodeType)
{
  return nodeType == AST_AND || nodeType == AST_OR;
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
    validateAssignmentTypes(node);
    break;

  case AST_ASSIGN_VEC:
    if (node->sons[0] && node->sons[0]->symbol)
    {
      if (node->sons[0]->symbol->type != SYMBOL_VECTOR)
      {
        cerr << "Semantic error: Identifier " << node->sons[0]->symbol->text << " must be a vector at line " << node->line << endl;
        semanticErrors++;
      }

      if (node->sons[1])
      {
        int indexDataType = inferExpressionType(node->sons[1]);
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

  case AST_IF:
  case AST_IFELSE:
    if (node->sons[0])
    {
      int conditionType = inferExpressionType(node->sons[0]);
      if (conditionType != DATATYPE_BOOL)
      {
        cerr << "Semantic error: Condition in 'if' must be boolean at line " << node->line << endl;
        semanticErrors++;
      }
    }
    checkUsageConsistency(node->sons[1]);
    if (node->type == AST_IFELSE && node->sons[2])
    {
      checkUsageConsistency(node->sons[2]);
    }
    break;

  case AST_WHILE:
    if (node->sons[0])
    {
      int conditionType = inferExpressionType(node->sons[0]);
      if (conditionType != DATATYPE_BOOL)
      {
        cerr << "Semantic error: Condition in 'while' must be boolean at line " << node->line << endl;
        semanticErrors++;
      }
    }
    checkUsageConsistency(node->sons[1]);
    break;

  case AST_FUNCTION_DEC:
    if (node->sons[3])
    {
      int expectedReturnType = node->dataType;
      checkReturnStatements(node->sons[3], expectedReturnType);
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
    cerr << "Semantic error: Vector size must be an integer literal, found '" << node->sons[2]->symbol->text << "' at line " << node->line << endl;
    semanticErrors++;
    return;
  }

  int declaredSize = parseVectorSize(node->sons[2]->symbol->text, node->line);
  if (declaredSize == -1)
  {
    return;
  }

  int actualSize = countVectorElements(node->sons[3]);

  if (declaredSize != actualSize)
  {
    cerr << "Semantic error: Mismatch in vector initialization size at line " << node->line << ". Declared: " << declaredSize << ", Actual: " << actualSize << endl;
    semanticErrors++;
  }

  if (!validateVectorTypes(node->sons[0]->dataType, node->sons[3]))
  {
    cerr << "Semantic error: Incompatible types in vector initialization at line " << node->line << endl;
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

  int declaredCount = countFunctionParameters(declaredParams);
  int passedCount = countFunctionParameters(passedArgs);

  if (declaredCount != passedCount)
  {
    cerr << "Semantic error: Mismatch in number of arguments for function '" << functionSymbol->text << "' at line " << node->line << ". Declared: " << declaredCount << ", Passed: " << passedCount << endl;
    semanticErrors++;
    return;
  }

  AST *declared = declaredParams;
  AST *passed = passedArgs;

  while (declared && passed)
  {
    int declaredType = declared->sons[1] ? declared->sons[1]->symbol->dataType : -1;
    int passedType = passed->sons[0] ? passed->sons[0]->dataType : -1;

    if (declaredType == -1 || passedType == -1)
    {
      cerr << "Semantic error: Invalid type in function arguments at line " << node->line << endl;
      semanticErrors++;
      return;
    }

    if (!areTypesCompatible(declaredType, passedType))
    {
      cerr << "Semantic error: Type mismatch in arguments for function '" << functionSymbol->text << "' at line " << node->line << endl;
      semanticErrors++;
    }

    declared = declared->sons[2];
    passed = passed->sons[1];
  }
}

int parseVectorSize(const string &sizeText, int line)
{
  try
  {
    string sanitizedText = (sizeText[0] == '#') ? sizeText.substr(1) : sizeText;
    return stoi(sanitizedText);
  }
  catch (...)
  {
    cerr << "Semantic error: Invalid vector size '" << sizeText << "' at line " << line << endl;
    semanticErrors++;
    return -1;
  }
}

int countFunctionParameters(AST *node)
{
  if (!node)
    return 0;

  if (node->type == AST_NONEMPTY_FUNC_CALL_PARAMS)
  {
    return 1 + countFunctionParameters(node->sons[1]);
  }

  if (node->type == AST_FUNC_CALL_PARAMS)
  {
    return (node->sons[0] ? 1 : 0) + countFunctionParameters(node->sons[1]);
  }

  if (node->type == AST_FUNCTION_DEC_PARAMS)
  {
    int count = (node->sons[0] && node->sons[1]) ? 1 : 0;
    count += countFunctionParameters(node->sons[2]);
    return count;
  }

  if (node->type == AST_NONEMPTY_FUNCTION_DEC_PARAMS)
  {
    return 1 + countFunctionParameters(node->sons[2]);
  }

  return 0;
}

void validateUndeclaredIdentifiers()
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

void validateReturnType(AST *node, int expectedReturnType)
{
  if (!node || node->type != AST_RETURN)
    return;

  if (node->sons[0])
  {
    int returnType = inferExpressionType(node->sons[0]);

    if (returnType != expectedReturnType)
    {
      cerr << "Semantic error: Return type mismatch at line " << node->line << ". Expected: " << expectedReturnType << ", Found: " << returnType << endl;
      semanticErrors++;
    }
  }
  else if (expectedReturnType != -1)
  {
    cerr << "Semantic error: Missing return value for function at line " << node->line << ". Expected type: " << expectedReturnType << endl;
    semanticErrors++;
  }
}

void checkReturnStatements(AST *node, int expectedReturnType)
{
  if (!node)
    return;

  if (node->type == AST_RETURN)
  {
    validateReturnType(node, expectedReturnType);
  }

  for (AST *child : node->sons)
  {
    if (child)
      checkReturnStatements(child, expectedReturnType);
  }
}

bool areTypesCompatible(int type1, int type2)
{
  return (type1 == type2) || (isInteger(type1) && isInteger(type2));
}
