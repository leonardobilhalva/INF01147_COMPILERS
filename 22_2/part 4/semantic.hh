// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "mapContainer.h"
#include "ast.hh"

#endif

int verifySemantic(AST *root);

void setIdentifierTypes(AST *node);
void setDeclarationTypes(AST *node);
void validateUndeclaredIdentifiers();

void setDataTypes(AST *node);
void checkUsageConsistency(AST *node);
int isInteger(int datatype);
void validateAssignmentTypes(AST *node);
int inferExpressionType(AST *node);
bool isArithmeticOperation(int nodeType);
bool isRelationalOperation(int nodeType);
bool isLogicalOperation(int nodeType);

void validateVectorInitialization(AST *node);
int countVectorElements(AST *node);
bool validateVectorTypes(int vectorType, AST *elements);

void validateFunctionArguments(AST *node);
int parseVectorSize(const string &sizeText, int line);
int countFunctionParameters(AST *node);
AST *findFunctionDeclaration(Symbol *symbol, AST *node = nullptr);
bool validateParameterTypes(AST *declared, AST *passed);
void validateReturnType(AST *node, int expectedReturnType);
void checkReturnStatements(AST *node, int expectedReturnType);
bool areTypesCompatible(int type1, int type2);
