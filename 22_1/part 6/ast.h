#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"

#define MAX_SONS 4

#define AST_SYMBOL 1
#define AST_VALUE 2
#define AST_ADD 3
#define AST_SUB 4
#define AST_MUL 5
#define AST_DIV 6
#define AST_GTR 7
#define AST_LS 8
#define AST_AND 9
#define AST_OR 10
#define AST_NEG 11
#define AST_OPLE 12
#define AST_OPGE 13
#define AST_OPEQ 14
#define AST_OPDIF 15
#define AST_FUNCCALL 16
#define AST_VARDECL 17
#define AST_VECTORDECL 18
#define AST_PARAM 19
#define AST_PARAM_LIST 20
#define AST_ARG_LIST 21
#define AST_ARG 22
#define AST_BLOCK 23
#define AST_BLOCK_LIST 44
#define AST_ASSIGN 24
#define AST_ASSIGN_VECTOR 25
#define AST_IF 26
#define AST_IFELSE 27
#define AST_WHILE 28
#define AST_READ 29
#define AST_PRINT 30
#define AST_PRINT_EXPR 31
#define AST_RETURN 32
#define AST_PARENS 33
#define AST_VECTOR 34
#define AST_KWCHAR 35
#define AST_KWINT 36
#define AST_KWFLOAT 37
#define AST_KWBOOL 38
#define AST_FUNCDECL 39
#define AST_HEAD 40
#define AST_HEAD_GLOBAL_VAR 41
#define AST_GLOBAL_VECTOR 42
#define AST_GLOBAL_VECTOR_LOOP 43
#define AST_HEAD_TAIL 45

typedef struct astnode
{
    int type;
    int line;
    int dataType;
    HASH_NODE *symbol;
    struct astnode *son[MAX_SONS];
} AST;

AST *astCreate(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3, int line);
void astPrint(AST *node, int level);
void astPrintCode(AST *node, FILE *output);
const char* astTypeToString(int type);

#endif