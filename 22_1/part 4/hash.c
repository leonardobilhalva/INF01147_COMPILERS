// Leonardo Barros Bilhalva - 315768
// Code from teacher

#include "hash.h"
#include <string.h>
#include <stdlib.h>

HASH_NODE *Table[HASH_SIZE];

void hashInit(void)
{
    int i;
    for (i = 0; i < HASH_SIZE; ++i)
        Table[i] = 0;
}

int hashAddress(char *text)
{
    int address = 1;
    int i;
    for (i = 0; i < strlen(text); ++i)
        address = (address * text[i]) % HASH_SIZE + 1;
    return address - 1;
}

HASH_NODE *hashFind(char *text)
{
    HASH_NODE *node;
    int address = hashAddress(text);
    for (node = Table[address]; node; node = node->next)
        if (strcmp(node->text, text) == 0)
            return node;
    return 0;
}

HASH_NODE *hashInsert(char *text, int type, int datatype)
{
    HASH_NODE *newnode;
    int address = hashAddress(text);

    if ((newnode = hashFind(text)) != 0)
        return newnode;

    newnode = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));
    newnode->type = type;

    switch (newnode->type)
    {
    case SYMBOL_LIT_CHAR:
        newnode->dataType = DATATYPE_CHAR;
        break;
    case SYMBOL_LIT_INT:
        newnode->dataType = DATATYPE_INT;
        break;
    case SYMBOL_LIT_FLOAT:
        newnode->dataType = DATATYPE_FLOAT;
        break;
    case SYMBOL_LIT_TRUE:
        newnode->dataType = DATATYPE_BOOL;
        break;
    case SYMBOL_LIT_FALSE:
        newnode->dataType = DATATYPE_BOOL;
        break;
    default:
        break;
    }

    newnode->text = (char *)calloc(strlen(text) + 1, sizeof(char));
    strcpy(newnode->text, text);
    newnode->next = Table[address];
    Table[address] = newnode;
    return newnode;
}

void hashPrint(void)
{
    int i;
    HASH_NODE *node;
    for (i = 0; i < HASH_SIZE; ++i)
        for (node = Table[i]; node; node = node->next)
            printf("Table[%d] has %s\n", i, node->text);
}