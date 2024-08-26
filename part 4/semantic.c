// Leonardo Barros Bilhalva - 315768

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "semantic.h"

int semanticErrors = 0;
AST *ROOT;

int verifySemantic(AST *root)
{

    ROOT = root;

    setIdentifierTypes(root);
    setNodeTypes(root);
    checkUndeclared();
    checkUsage(root);
    checkReturns(root);

    return semanticErrors;
}

void checkUndeclared()
{
    HASH_NODE *node;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (Table[i] != NULL)
        {
            for (node = Table[i]; node != NULL; node = node->next)
            {
                if (node->type == SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "SEMANTIC ERROR - Variable %s not declared.\n", node->text);
                    semanticErrors++;
                }
            }
        }
    }
}

void setIdentifierTypes(AST *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {

    case AST_VARDECL:
        if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Variable %s redeclaration.\n", node->line, node->son[1]->symbol->text);
            semanticErrors++;
        }
        else
        {
            node->son[1]->symbol->type = SYMBOL_VAR;
            if (node->son[0]->type == AST_KWCHAR)
                node->son[1]->symbol->dataType = DATATYPE_CHAR;
            else if (node->son[0]->type == AST_KWINT)
                node->son[1]->symbol->dataType = DATATYPE_INT;
            else if (node->son[0]->type == AST_KWFLOAT)
                node->son[1]->symbol->dataType = DATATYPE_FLOAT;
            else if (node->son[0]->type == AST_KWBOOL)
                node->son[1]->symbol->dataType = DATATYPE_BOOL;
        }
        if (!isDataTypeCompatible(node->son[1]->symbol->dataType, node->son[2]->symbol->dataType))
        {

            fprintf(stderr, "SEMANTIC ERROR - Line %d: Variable declaration with mixed dataypes\n", node->line);
            semanticErrors++;
        }
        break;
    case AST_VECTORDECL:
        if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Vector %s redeclaration.\n", node->line, node->son[1]->symbol->text);
            semanticErrors++;
        }
        else
        {
            node->son[1]->symbol->type = SYMBOL_VECTOR;
            if (node->son[0]->type == AST_KWCHAR)
                node->son[1]->symbol->dataType = DATATYPE_CHAR;
            else if (node->son[0]->type == AST_KWINT)
                node->son[1]->symbol->dataType = DATATYPE_INT;
            else if (node->son[0]->type == AST_KWFLOAT)
                node->son[1]->symbol->dataType = DATATYPE_FLOAT;
        }
        if (!checkElementVector(node->son[3], node->son[1]->symbol->dataType))
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Vector declaration with mixed datatype on elements\n", node->line);
            semanticErrors++;
        }
        break;

    case AST_FUNCDECL:
        if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Function %s redeclaration.\n", node->line, node->son[1]->symbol->text);
            semanticErrors++;
        }
        else
        {
            node->son[1]->symbol->type = SYMBOL_FUNC;
            if (node->son[0]->type == AST_KWCHAR)
                node->son[1]->symbol->dataType = DATATYPE_CHAR;
            else if (node->son[0]->type == AST_KWINT)
                node->son[1]->symbol->dataType = DATATYPE_INT;
            else if (node->son[0]->type == AST_KWFLOAT)
                node->son[1]->symbol->dataType = DATATYPE_FLOAT;
        }
        break;
    case AST_PARAM:
        if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Parameter %s redeclaration.\n", node->line, node->son[1]->symbol->text);
            semanticErrors++;
        }
        else
        {
            node->son[1]->symbol->type = SYMBOL_PARAM;
            if (node->son[0]->type == AST_KWCHAR)
                node->son[1]->symbol->dataType = DATATYPE_CHAR;
            else if (node->son[0]->type == AST_KWINT)
                node->son[1]->symbol->dataType = DATATYPE_INT;
            else if (node->son[0]->type == AST_KWFLOAT)
                node->son[1]->symbol->dataType = DATATYPE_FLOAT;
        }
        break;
    case AST_PARAM_LIST:
        if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Parameter %s redeclaration.\n", node->line, node->son[1]->symbol->text);
            semanticErrors++;
        }
        else
        {
            node->son[1]->symbol->type = SYMBOL_PARAM;
            if (node->son[0]->type == AST_KWCHAR)
                node->son[1]->symbol->dataType = DATATYPE_CHAR;
            else if (node->son[0]->type == AST_KWINT)
                node->son[1]->symbol->dataType = DATATYPE_INT;
            else if (node->son[0]->type == AST_KWFLOAT)
                node->son[1]->symbol->dataType = DATATYPE_FLOAT;
        }
        break;
    default:
        break;
    }
    for (int i = 0; i < MAX_SONS; i++)
    {
        setIdentifierTypes(node->son[i]);
    }
}

void setNodeTypes(AST *node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < MAX_SONS; i++)
    {
        setNodeTypes(node->son[i]);
    }
    if (node->type == AST_SYMBOL)
    {
        node->dataType = node->symbol->dataType;
    }
    else if (node->type == AST_FUNCCALL || node->type == AST_VECTOR)
    {
        node->dataType = node->son[0]->symbol->dataType;
    }
    else if (node->type == AST_PARENS)
    {
        node->dataType = node->son[0]->dataType;
    }
    else if (isAritmeticOp(node->type))
    {
        AST *son0 = node->son[0];
        AST *son1 = node->son[1];
        if (!isDataTypeCompatible(son0->dataType, son1->dataType) || son0->dataType == DATATYPE_BOOL || son1->dataType == DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Arithmetic operation with incompatible datatypes.\n", node->line);
            semanticErrors++;
        }
        node->dataType = greaterDatatype(son0->dataType, son1->dataType);
    }
    else if (isRelationalOp(node->type))
    {
        if (!isNumerical(node->son[0]->dataType) || !isNumerical(node->son[1]->dataType))
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Relational operation with incompatible datatypes.\n", node->line);
            semanticErrors++;
        }
        node->dataType = DATATYPE_BOOL;
    }
    else if (isLogicalOp(node->type))
    {
        if (node->type == AST_NEG)
        {
            if (node->son[0]->dataType != DATATYPE_BOOL)
            {
                fprintf(stderr, "SEMANTIC ERROR - Line %d: Logical operation with incompatible datatypes.\n", node->line);
                semanticErrors++;
            }
        }
        else if (node->son[0]->dataType != DATATYPE_BOOL || node->son[1]->dataType != DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Logical operation with incompatible datatypes.\n", node->line);
            semanticErrors++;
        }
        node->dataType = DATATYPE_BOOL;
    }
}

void checkUsage(AST *node)
{
    if (node == NULL)
        return;
    switch (node->type)
    {
    case AST_ASSIGN:
        if (node->son[0]->symbol->type != SYMBOL_VAR)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Attribution must be to a scalar variable.\n", node->line);
            semanticErrors++;
        }
        if (!isDataTypeCompatible(node->son[0]->symbol->dataType, node->son[1]->dataType))
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Attribution with incompatible datatype.\n", node->line);
            semanticErrors++;
        }
        break;
    case AST_ASSIGN_VECTOR:
        if (node->son[0]->symbol->type != SYMBOL_VECTOR)
        {
            fprintf(stderr, "Semantic ERROR - %d: Indexing only allowed for vectors.\n", node->line);
            semanticErrors++;
        }
        if (!isDataTypeCompatible(node->son[0]->symbol->dataType, node->son[1]->dataType))
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Attribution with incompatible datatype.\n", node->line);
            semanticErrors++;
        }
        if (!isInteger(node->son[0]->dataType))
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Index must be an integer.\n", node->line);
            semanticErrors++;
        }
        break;
    case AST_FUNCCALL:
        validateFunction(node);
        break;
    case AST_IF:
        if (node->son[0]->dataType != DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Condition must be a boolean expression.\n", node->line);
            semanticErrors++;
        }
        break;
    case AST_IFELSE:
        if (node->son[0]->dataType != DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Condition must be a boolean expression.\n", node->line);
            semanticErrors++;
        }
        break;
    case AST_WHILE:
        if (node->son[0]->dataType != DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Condition must be a boolean expression.\n", node->line);
            semanticErrors++;
        }
        break;
    case AST_READ:
        if (node->son[1]->symbol->type != SYMBOL_VAR)
        {
            fprintf(stderr, "Semantic ERROR - %d: Read only allowed to scalar variables.\n", node->line);
            semanticErrors++;
        }
        if (!isDataTypeCompatible(node->son[0]->dataType, node->son[1]->symbol->dataType))
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Read operation with incompatible types.\n", node->line);
            semanticErrors++;
        }
        break;
    case AST_PRINT_EXPR:
        if (node->son[1]->type == AST_SYMBOL)
        {
            if (node->son[1]->symbol->type == SYMBOL_FUNC)
            {
                fprintf(stderr, "SEMANTIC ERROR - Line %d: Cannot print a function.\n", node->line);
                semanticErrors++;
            }
            else if (node->son[1]->symbol->type == SYMBOL_VECTOR)
            {
                fprintf(stderr, "SEMANTIC ERROR - Line %d: Cannot print a vector.\n", node->line);
                semanticErrors++;
            }
            if (!isDataTypeCompatible(node->son[1]->symbol->dataType, node->son[0]->dataType))
            {
                fprintf(stderr, "SEMANTIC ERROR - Line %d: Incompatible types in print expression.\n", node->line);
                semanticErrors++;
            }
        }
        break;
    case AST_PRINT:
        if (node->son[0]->type == AST_SYMBOL)
        {
            if (node->son[0]->symbol->type == SYMBOL_FUNC)
            {
                fprintf(stderr, "SEMANTIC ERROR - Line %d: Cannot print a function.\n", node->line);
                semanticErrors++;
            }
            else if (node->son[0]->symbol->type == SYMBOL_VECTOR)
            {
                fprintf(stderr, "SEMANTIC ERROR - Line %d: Cannot print a vector.\n", node->line);
                semanticErrors++;
            }
        }
        break;
    default:
        break;
    }

    for (int i = 0; i < MAX_SONS; i++)
        checkUsage(node->son[i]);
}

int isDataTypeCompatible(int datatype0, int datatype1)
{
    return (isInteger(datatype0) && isInteger(datatype1)) || (datatype0 == datatype1);
}

int isInteger(int datatype)
{
    return (datatype == DATATYPE_INT || datatype == DATATYPE_CHAR);
}

int isNumerical(int datatype)
{
    return (isInteger(datatype) || datatype == DATATYPE_FLOAT);
}

bool checkElementVector(AST *node, int dataType)
{
    if (node != NULL)
    {
        if (!isDataTypeCompatible(node->son[0]->symbol->dataType, dataType))
            return false;
        if (node->son[1] != NULL)
        {
            return checkElementVector(node->son[1], dataType);
        }
    }
    return true;
}

int greaterDatatype(int type1, int type2)
{
    return type1 > type2 ? type1 : type2;
}

int isAritmeticOp(int nodetype)
{
    return (nodetype == AST_MUL || nodetype == AST_DIV || nodetype == AST_ADD || nodetype == AST_SUB);
}

int isRelationalOp(int nodetype)
{
    return (nodetype == AST_GTR || nodetype == AST_LS || nodetype == AST_OPGE || nodetype == AST_OPLE || nodetype == AST_OPEQ || nodetype == AST_OPDIF);
}

int isLogicalOp(int nodetype)
{
    return (nodetype == AST_AND || nodetype == AST_OR || nodetype == AST_NEG);
}

void isReturnCompatible(AST *node, int expectedType)
{
    if (node == NULL)
        return;

    if (node->type == AST_RETURN)
    {
        if (node->son[0] == NULL)
        {
            fprintf(stderr, "Semantic ERROR - Line %d: Return statement with no expression.\n", node->line);
            semanticErrors++;
            return;
        }

        if (!isDataTypeCompatible(expectedType, node->son[0]->dataType))
        {
            fprintf(stderr, "Semantic ERROR - Line %d: Return statement with incompatible datatype.\n", node->line);
            semanticErrors++;
        }
    }

    for (int i = 0; i < MAX_SONS; i++)
    {
        if (node->son[i] != NULL)
        {
            isReturnCompatible(node->son[i], expectedType);
        }
    }
}

void checkReturns(AST *node)
{
    if (node == NULL)
        return;

    if (node->type == AST_FUNCDECL)
    {
        if (node->son[1] != NULL && node->son[1]->symbol != NULL)
        {
            isReturnCompatible(node->son[3], node->son[1]->symbol->dataType);
        }
        else
        {
            fprintf(stderr, "Semantic ERROR - Line %d: No valid symbol for function.\n", node->line);
            semanticErrors++;
        }
    }

    for (int i = 0; i < MAX_SONS; i++)
    {
        if (node->son[i] != NULL)
        {
            checkReturns(node->son[i]);
        }
    }
}

void validateFunction(AST *node)
{
    AST *declaration = findFunctionDeclaration(node->son[0]->symbol->text, ROOT);
    if (declaration == NULL)
    {
        fprintf(stderr, "SEMANTIC ERROR - Line %d: Only functions can be called.\n", node->line);
        semanticErrors++;
    }
    else if (checkNumberOfArguments(node->son[1], declaration->son[2]))
    {
        compareCalledArguments(node->son[1], declaration->son[2]);
    }
}

AST *findFunctionDeclaration(char *name, AST *node)
{
    if (node->son[1] != NULL && node->type == AST_FUNCDECL)
        if (strcmp(node->son[1]->symbol->text, name) == 0)
            return node;

    for (int i = 0; i < MAX_SONS; i++)
    {
        if (node->son[i] == NULL)
            return NULL;
        AST *findingAST = findFunctionDeclaration(name, node->son[i]);
        if (findingAST != NULL)
            return findingAST;
    }
    return NULL;
}

bool checkNumberOfArguments(AST *node, AST *declaration)
{
    int numberOfCalledArguments = getNumberOfArguments(node);
    int numberOfDeclaredArguments = getNumberOfArguments(declaration);
    if (numberOfCalledArguments != numberOfDeclaredArguments)
    {
        fprintf(stderr, "SEMANTIC ERROR - Line %d: Incompatible number of arguments.\n", node->line);
        semanticErrors++;
        return false;
    }
    return true;
}

int getNumberOfArguments(AST *node)
{
    if (node == NULL)
        return 0;
    if (node->son[1] != NULL)
        return 1 + getNumberOfArguments(node->son[1]);
    else
        return 0;
}

void compareCalledArguments(AST *node, AST *declaration)
{
    if (node->son[0] != NULL)
    {
        if (!isDataTypeCompatible(node->son[0]->dataType, declaration->son[1]->symbol->dataType))
        {
            fprintf(stderr, "SEMANTIC ERROR - Line %d: Incompatible argument datatypes\n", node->line);
            semanticErrors++;
        }
        if (node->son[0]->type == AST_SYMBOL)
        {
            if (node->son[0]->symbol->type == SYMBOL_FUNC)
            {
                fprintf(stderr, "SEMANTIC ERROR - Line %d: Function cannot be argument\n", node->line);
                semanticErrors++;
            }
            else if (node->son[0]->symbol->type == SYMBOL_VECTOR)
            {
                fprintf(stderr, "SEMANTIC ERROR - Line %d: Vector cannot be argument\n", node->line);
                semanticErrors++;
            }
        }
        if (node->son[1] != NULL)
            compareCalledArguments(node->son[1], declaration->son[2]);
    }
}