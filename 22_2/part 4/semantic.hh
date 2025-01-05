// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "mapContainer.h"
#include "ast.hh"

#endif

int verifySemantic(AST *root);

void setIdentifierTypes(AST *node);
void setDataTypes(AST *node);
void checkUsageConsistency(AST *node);
void validateUndeclaredIdentifiers();

void validateAssignmentTypes(AST *node);
int inferExpressionType(AST *node);
bool isArithmeticOperation(int nodeType);
bool isRelationalOperation(int nodeType);
bool isLogicalOperation(int nodeType);
int isInteger(int datatype);

void validateVectorInitialization(AST *node);
int countVectorElements(AST *node);
bool validateVectorTypes(int vectorType, AST *elements);

AST *findFunctionDeclaration(Symbol *symbol, AST *node = nullptr);
void validateFunctionArguments(AST *node);
int parseVectorSize(const string &sizeText, int line);
int countFunctionParameters(AST *node);

void validateReturnType(AST *node, int expectedReturnType);
void checkReturnStatements(AST *node, int expectedReturnType);

bool areTypesCompatible(int type1, int type2);
