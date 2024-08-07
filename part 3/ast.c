#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

AST *astCreate(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3) {
    AST *newnode;
    newnode = (AST *)calloc(1, sizeof(AST));
    if (!newnode) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = s0;
    newnode->son[1] = s1;
    newnode->son[2] = s2;
    newnode->son[3] = s3;
    fprintf(stderr, "Created node of type %d\n", type);
    return newnode;
}

void astPrint(AST *node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++)
        fprintf(stderr, "  ");

    switch (node->type) {
    case AST_SYMBOL:
        fprintf(stderr, "AST_SYMBOL");
        break;
    case AST_VALUE:
        fprintf(stderr, "AST_VALUE");
        break;
    case AST_ADD:
        fprintf(stderr, "AST_ADD");
        break;
    case AST_SUB:
        fprintf(stderr, "AST_SUB");
        break;
    case AST_MUL:
        fprintf(stderr, "AST_MUL");
        break;
    case AST_DIV:
        fprintf(stderr, "AST_DIV");
        break;
    case AST_GTR:
        fprintf(stderr, "AST_GTR");
        break;
    case AST_LS:
        fprintf(stderr, "AST_LS");
        break;
    case AST_AND:
        fprintf(stderr, "AST_AND");
        break;
    case AST_OR:
        fprintf(stderr, "AST_OR");
        break;
    case AST_NEG:
        fprintf(stderr, "AST_NEG");
        break;
    case AST_OPLE:
        fprintf(stderr, "AST_OPLE");
        break;
    case AST_OPGE:
        fprintf(stderr, "AST_OPGE");
        break;
    case AST_OPEQ:
        fprintf(stderr, "AST_OPEQ");
        break;
    case AST_OPDIF:
        fprintf(stderr, "AST_OPDIF");
        break;
    case AST_FUNCCALL:
        fprintf(stderr, "AST_FUNCCALL");
        break;
    case AST_VARDECL:
        fprintf(stderr, "AST_VARDECL");
        break;
    case AST_VECTORDECL:
        fprintf(stderr, "AST_VECTORDECL");
        break;
    case AST_PARAM:
        fprintf(stderr, "AST_PARAM");
        break;
    case AST_PARAM_LIST:
        fprintf(stderr, "AST_PARAM_LIST");
        break;
    case AST_ARG_LIST:
        fprintf(stderr, "AST_ARG_LIST");
        break;
    case AST_ARG:
        fprintf(stderr, "AST_ARG");
        break;
    case AST_BLOCK:
        fprintf(stderr, "AST_BLOCK");
        break;
    case AST_ASSIGN:
        fprintf(stderr, "AST_ASSIGN");
        break;
    case AST_ASSIGN_VECTOR:
        fprintf(stderr, "AST_ASSIGN_VECTOR");
        break;
    case AST_IF:
        fprintf(stderr, "AST_IF");
        break;
    case AST_IFELSE:
        fprintf(stderr, "AST_IFELSE");
        break;
    case AST_WHILE:
        fprintf(stderr, "AST_WHILE");
        break;
    case AST_READ:
        fprintf(stderr, "AST_READ");
        break;
    case AST_PRINT:
        fprintf(stderr, "AST_PRINT");
        break;
    case AST_PRINT_EXPR:
        fprintf(stderr, "AST_PRINT_EXPR");
        break;
    case AST_RETURN:
        fprintf(stderr, "AST_RETURN");
        break;
    case AST_PARENS:
        fprintf(stderr, "AST_PARENS");
        break;
    case AST_VECTOR:
        fprintf(stderr, "AST_VECTOR");
        break;
    case AST_KWCHAR:
        fprintf(stderr, "AST_KWCHAR");
        break;
    case AST_KWINT:
        fprintf(stderr, "AST_KWINT");
        break;
    case AST_KWFLOAT:
        fprintf(stderr, "AST_KWFLOAT");
        break;
    case AST_KWBOOL:
        fprintf(stderr, "AST_KWBOOL");
        break;
    case AST_FUNCDECL:
        fprintf(stderr, "AST_FUNCDECL");
        break;
    case AST_HEAD:
        fprintf(stderr, "AST_HEAD");
        break;
    case AST_HEAD_GLOBAL_VAR:
        fprintf(stderr, "AST_HEAD_GLOBAL_VAR");
        break;
    case AST_GLOBAL_VECTOR:
        fprintf(stderr, "AST_GLOBAL_VECTOR");
        break;
    case AST_GLOBAL_VECTOR_LOOP:
        fprintf(stderr, "AST_GLOBAL_VECTOR_LOOP");
        break;
    case AST_HEAD_TAIL:
        fprintf(stderr, "AST_HEAD_TAIL");
        break;
    default:
        fprintf(stderr, "AST_UNKNOWN");
        break;
    }

    if (node->symbol)
        fprintf(stderr, ", %s\n", node->symbol->text);
    else
        fprintf(stderr, "\n");

    for (int i = 0; i < MAX_SONS; ++i) {
        if (node->son[i]) {
            fprintf(stderr, "Printing child %d of node type %s\n", i, astTypeToString(node->type));
            astPrint(node->son[i], level + 1);
        }
    }
}

const char* astTypeToString(int type) {
    switch(type) {
        case AST_SYMBOL: return "AST_SYMBOL";
        case AST_VALUE: return "AST_VALUE";
        case AST_ADD: return "AST_ADD";
        case AST_SUB: return "AST_SUB";
        case AST_MUL: return "AST_MUL";
        case AST_DIV: return "AST_DIV";
        case AST_GTR: return "AST_GTR";
        case AST_LS: return "AST_LS";
        case AST_AND: return "AST_AND";
        case AST_OR: return "AST_OR";
        case AST_NEG: return "AST_NEG";
        case AST_OPLE: return "AST_OPLE";
        case AST_OPGE: return "AST_OPGE";
        case AST_OPEQ: return "AST_OPEQ";
        case AST_OPDIF: return "AST_OPDIF";
        case AST_FUNCCALL: return "AST_FUNCCALL";
        case AST_VARDECL: return "AST_VARDECL";
        case AST_VECTORDECL: return "AST_VECTORDECL";
        case AST_PARAM: return "AST_PARAM";
        case AST_PARAM_LIST: return "AST_PARAM_LIST";
        case AST_ARG_LIST: return "AST_ARG_LIST";
        case AST_ARG: return "AST_ARG";
        case AST_BLOCK: return "AST_BLOCK";
        case AST_ASSIGN: return "AST_ASSIGN";
        case AST_ASSIGN_VECTOR: return "AST_ASSIGN_VECTOR";
        case AST_IF: return "AST_IF";
        case AST_IFELSE: return "AST_IFELSE";
        case AST_WHILE: return "AST_WHILE";
        case AST_READ: return "AST_READ";
        case AST_PRINT: return "AST_PRINT";
        case AST_PRINT_EXPR: return "AST_PRINT_EXPR";
        case AST_RETURN: return "AST_RETURN";
        case AST_PARENS: return "AST_PARENS";
        case AST_VECTOR: return "AST_VECTOR";
        case AST_KWCHAR: return "AST_KWCHAR";
        case AST_KWINT: return "AST_KWINT";
        case AST_KWFLOAT: return "AST_KWFLOAT";
        case AST_KWBOOL: return "AST_KWBOOL";
        case AST_FUNCDECL: return "AST_FUNCDECL";
        case AST_HEAD: return "AST_HEAD";
        case AST_HEAD_GLOBAL_VAR: return "AST_HEAD_GLOBAL_VAR";
        case AST_GLOBAL_VECTOR: return "AST_GLOBAL_VECTOR";
        case AST_GLOBAL_VECTOR_LOOP: return "AST_GLOBAL_VECTOR_LOOP";
        case AST_HEAD_TAIL: return "AST_HEAD_TAIL";
        default: return "AST_UNKNOWN";
    }
}

void astPrintCode(AST *node, FILE *output) {
    if (!node) return;

    fprintf(stderr, "Printing node of type %s\n", astTypeToString(node->type));

    switch (node->type) {
        case AST_SYMBOL:
            fprintf(output, " %s ", node->symbol->text);
            break;
        case AST_VALUE:
            astPrintCode(node->son[0], output);
            break;
        case AST_ADD:
            astPrintCode(node->son[0], output);
            fprintf(output, " + ");
            astPrintCode(node->son[1], output);
            break;
        case AST_SUB:
            astPrintCode(node->son[0], output);
            fprintf(output, " - ");
            astPrintCode(node->son[1], output);
            break;
        case AST_MUL:
            astPrintCode(node->son[0], output);
            fprintf(output, " * ");
            astPrintCode(node->son[1], output);
            break;
        case AST_DIV:
            astPrintCode(node->son[0], output);
            fprintf(output, " / ");
            astPrintCode(node->son[1], output);
            break;
        case AST_GTR:
            astPrintCode(node->son[0], output);
            fprintf(output, " > ");
            astPrintCode(node->son[1], output);
            break;
        case AST_LS:
            astPrintCode(node->son[0], output);
            fprintf(output, " < ");
            astPrintCode(node->son[1], output);
            break;
        case AST_AND:
            astPrintCode(node->son[0], output);
            fprintf(output, " & ");
            astPrintCode(node->son[1], output);
            break;
        case AST_OR:
            astPrintCode(node->son[0], output);
            fprintf(output, " | ");
            astPrintCode(node->son[1], output);
            break;
        case AST_NEG:
            fprintf(output, " ~ ");
            astPrintCode(node->son[0], output);
            break;
        case AST_OPLE:
            astPrintCode(node->son[0], output);
            fprintf(output, " <= ");
            astPrintCode(node->son[1], output);
            break;
        case AST_OPGE:
            astPrintCode(node->son[0], output);
            fprintf(output, " >= ");
            astPrintCode(node->son[1], output);
            break;
        case AST_OPEQ:
            astPrintCode(node->son[0], output);
            fprintf(output, " == ");
            astPrintCode(node->son[1], output);
            break;
        case AST_OPDIF:
            astPrintCode(node->son[0], output);
            fprintf(output, " != ");
            astPrintCode(node->son[1], output);
            break;

        case AST_FUNCCALL:
            astPrintCode(node->son[0], output);
            fprintf(output, " ( ");
            if (node->son[1] != 0)
            {
                astPrintCode(node->son[1], output);
            }
            fprintf(output, " ) ");
            break;
        case AST_VARDECL:
            astPrintCode(node->son[0], output);
            astPrintCode(node->son[1], output);
            fprintf(output, " = ");
            astPrintCode(node->son[2], output);
            break;
        case AST_VECTORDECL:
            astPrintCode(node->son[0], output);
            astPrintCode(node->son[1], output);
            fprintf(output, " [ ");
            astPrintCode(node->son[2], output);
            fprintf(output, " ] ");
            if (node->son[3] != 0)
            {
                astPrintCode(node->son[3], output);
            }
            break;

        case AST_GLOBAL_VECTOR:
            fprintf(output, " : ");
            astPrintCode(node->son[0], output);
            if (node->son[1] != 0)
            {
                astPrintCode(node->son[1], output);
            }
            break;
        case AST_GLOBAL_VECTOR_LOOP:
            astPrintCode(node->son[0], output);
            if (node->son[1] != 0)
            {
                astPrintCode(node->son[1], output);
            }
            break;
        case AST_PARAM:
            astPrintCode(node->son[0], output);
            astPrintCode(node->son[1], output);
            break;
        case AST_PARAM_LIST:
            astPrintCode(node->son[0], output);
            astPrintCode(node->son[1], output);
            fprintf(output, ", ");
            astPrintCode(node->son[2], output);
            break;
        case AST_ARG_LIST:
            astPrintCode(node->son[0], output);
            fprintf(output, ", ");
            astPrintCode(node->son[1], output);
            break;
        case AST_ARG:
            astPrintCode(node->son[0], output);
            break;
        case AST_BLOCK:
            fprintf(output, " { ");
            if (node->son[0] != 0)
            {
                astPrintCode(node->son[0], output);
            }
            fprintf(output, " } ");
            break;
        case AST_BLOCK_LIST:
            if (node->son[0] != 0)
            {
                astPrintCode(node->son[0], output);
            }
            else
            {
                fprintf(output, " ; ");
            }
            if (node->son[1] != 0)
            {
                astPrintCode(node->son[1], output);
            }
            break;
        case AST_ASSIGN:
            astPrintCode(node->son[0], output);
            fprintf(output, " = ");
            astPrintCode(node->son[1], output);
            fprintf(output, " ; ");
            fprintf(output, " \n ");
            break;
        case AST_ASSIGN_VECTOR:
            astPrintCode(node->son[0], output);
            fprintf(output, " [ ");
            astPrintCode(node->son[1], output);
            fprintf(output, " ]");
            fprintf(output, " = ");
            astPrintCode(node->son[2], output);
            fprintf(output, " ; ");
            fprintf(output, " \n ");
            break;
        case AST_IF:
            fprintf(output, "if ( ");
            astPrintCode(node->son[0], output);
            fprintf(output, " ) ");
            astPrintCode(node->son[1], output);
            break;
        case AST_IFELSE:
            fprintf(output, "if ( ");
            astPrintCode(node->son[0], output);
            fprintf(output, " ) ");
            astPrintCode(node->son[1], output);
            fprintf(output, " else ");
            astPrintCode(node->son[2], output);
            break;
        case AST_WHILE:
            fprintf(output, "while ( ");
            astPrintCode(node->son[0], output);
            fprintf(output, " ) ");
            astPrintCode(node->son[1], output);
            break;
        case AST_READ:
            fprintf(output, "read ");
            astPrintCode(node->son[0], output);
            astPrintCode(node->son[1], output);
            fprintf(output, " ; ");
            fprintf(output, " \n ");
            break;
        case AST_PRINT:
            fprintf(output, "print ");
            astPrintCode(node->son[0], output);
            fprintf(output, " ; ");
            fprintf(output, " \n ");
            break;
        case AST_PRINT_EXPR:
            fprintf(output, "print ");
            astPrintCode(node->son[0], output);
            astPrintCode(node->son[1], output);
            break;
        case AST_RETURN:
            fprintf(output, "return ");
            astPrintCode(node->son[0], output);
            fprintf(output, " ; ");
            fprintf(output, " \n ");
            break;
        case AST_PARENS:
            fprintf(output, " ( ");
            astPrintCode(node->son[0], output);
            fprintf(output, " ) ");
            break;
        case AST_VECTOR: 
            astPrintCode(node->son[0], output);
            fprintf(output, " [ ");
            astPrintCode(node->son[1], output);
            fprintf(output, "]");
            break;
        case AST_KWCHAR:
            fprintf(output, "char ");
            break;
        case AST_KWINT:
            fprintf(output, "int ");
            break;
        case AST_KWFLOAT:
            fprintf(output, "float ");
            break;
        case AST_KWBOOL:
            fprintf(output, "bool ");
            break;


        case AST_FUNCDECL:
            astPrintCode(node->son[0], output);
            astPrintCode(node->son[1], output);
            fprintf(output, " ( ");
            if (node->son[2] != 0)
            {
                astPrintCode(node->son[2], output);
            }
            fprintf(output, " ) ");
            astPrintCode(node->son[3], output);
            break;
        case AST_HEAD_GLOBAL_VAR:
            astPrintCode(node->son[0], output);
            fprintf(output, " ; ");
            if (node->son[1] != 0)
            {
                astPrintCode(node->son[1], output);
            }
            break;
        case AST_HEAD:
            astPrintCode(node->son[0], output);
            if (node->son[1] != 0)
            {
                astPrintCode(node->son[1], output);
            }
            break;
        case AST_HEAD_TAIL:
            if (node->son[0] != 0)
            {
                astPrintCode(node->son[0], output);
            }
            break;
        default:
            fprintf(stderr, "ERROR on %d\n", node->type);
            break;
    }
}