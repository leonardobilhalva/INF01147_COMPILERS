// Leonardo Barros Bilhalva - 315768

#ifndef TAC_HEADER
#define TAC_HEADER

#include "ast.h"

#define TAC_SYMBOL 0
#define TAC_ADD 1
#define TAC_SUB 2
#define TAC_MUL 3
#define TAC_DIV 4
#define TAC_GTR 5
#define TAC_LS 6
#define TAC_OPGE 7
#define TAC_OPLE 8
#define TAC_OPEQ 9
#define TAC_OPDIF 10
#define TAC_AND 11
#define TAC_OR 12
#define TAC_NEG 13
#define TAC_FUNCDECL 14
#define TAC_FUNCCALL 15
#define TAC_VECTOR 16
#define TAC_MOVE 17
#define TAC_MOVEVEC 18
#define TAC_PARAM 19
#define TAC_RETURN 20
#define TAC_PRINT 21
#define TAC_LABEL 22
#define TAC_IF 23
#define TAC_JUMP 24
#define TAC_FUNCSTART 25
#define TAC_FUNCEND 26
#define TAC_ARG 27
#define TAC_ASSIGNVEC 28
#define TAC_READ 31


typedef struct tac
{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac *prev;
    struct tac *next;
} tac;

tac *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
tac *tacRewind(tac *l);
void tacPrint(tac *tac);
void printAllTacs(tac *l);
tac *tacGenerateCode(AST *node, HASH_NODE *currentLoopLabel);

tac *createBinop(int type, tac *sons[]);
tac *createIf(tac *sons[]);
tac *createLoop(tac *sons[], HASH_NODE *label);
tac *createFunction(tac *symbol, tac *code, tac *rest);

#endif