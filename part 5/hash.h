// Leonardo Barros Bilhalva - 315768
// Code from teacher

#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>

#define HASH_SIZE 997

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_FLOAT 2
#define SYMBOL_LIT_CHAR 4
#define SYMBOL_LIT_STRING 5
#define SYMBOL_LIT_TRUE 6
#define SYMBOL_LIT_FALSE 7
#define SYMBOL_IDENTIFIER 8

#define SYMBOL_VAR 10
#define SYMBOL_VECTOR 11
#define SYMBOL_FUNC 12
#define SYMBOL_PARAM 13

#define DATATYPE_CHAR 20
#define DATATYPE_INT 21
#define DATATYPE_FLOAT 22
#define DATATYPE_BOOL 23

typedef struct hash_node
{
    int type;
    int dataType;
    char *text;
    struct hash_node *next;
} HASH_NODE;

extern HASH_NODE *Table[HASH_SIZE];

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type, int datatype);
void hashPrint(void);

int hashCheckUndeclared();
HASH_NODE *makeTemp();
HASH_NODE *makeLabel();

#endif