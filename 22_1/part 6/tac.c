// Leonardo Barros Bilhalva - 315768

#include "tac.h"
#include "hash.h"
#include "stdlib.h"

tac *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2)
{
    tac *n = (tac *)calloc(1, sizeof(tac));
    n->type = type;
    n->res = res;
    n->op1 = op1;
    n->op2 = op2;
    n->prev = NULL;
    n->next = NULL;
    return n;
}

tac *tacJoin(tac *l1, tac *l2)
{
    tac *tacJoined;
    if (!l1)
        return l2;
    if (!l2)
        return l1;
    tacJoined = l2;
    while (tacJoined->prev)
    {
        tacJoined = tacJoined->prev;
    }
    tacJoined->prev = l1;
    return l2;
}

void printAllTacs(tac *l)
{
    tac *tac;
    for (tac = l; tac; tac = tac->next)
        tacPrint(tac);
}

tac *tacRewind(tac *l)
{
    tac *tac;
    for (tac = l; tac->prev; tac = tac->prev)
        tac->prev->next = tac;
    return tac;
}

void tacPrint(tac *tac)
{

    if (tac->type == TAC_SYMBOL)
        return;

    fprintf(stderr, "TAC(");
    switch (tac->type)
    {
    case TAC_SYMBOL: fprintf(stderr, "SYMBOL("); break;
    case TAC_ADD: fprintf(stderr, "ADD("); break;
    case TAC_SUB: fprintf(stderr, "SUB("); break;
    case TAC_MUL: fprintf(stderr, "MUL("); break;
    case TAC_DIV: fprintf(stderr, "DIV("); break;
    case TAC_GTR: fprintf(stderr, "GTR("); break;
    case TAC_LS: fprintf(stderr, "LS("); break;
    case TAC_OPGE: fprintf(stderr, "OPGE("); break;   
    case TAC_OPLE:       fprintf(stderr, "OPLE("); break;
    case TAC_OPEQ:       fprintf(stderr, "OPEQ("); break;
    case TAC_OPDIF:      fprintf(stderr, "OPDIF("); break;
    case TAC_AND:        fprintf(stderr, "AND("); break;
    case TAC_OR:         fprintf(stderr, "OR("); break;
    case TAC_NEG:        fprintf(stderr, "NEG("); break;
    case TAC_FUNCDECL:   fprintf(stderr, "FUNCDECL("); break;
    case TAC_FUNCCALL:   fprintf(stderr, "FUNCCALL("); break;
    case TAC_VECTOR:     fprintf(stderr, "VECTOR("); break;
    case TAC_MOVE:       fprintf(stderr, "MOVE("); break;
    case TAC_MOVEVEC:    fprintf(stderr, "MOVEVEC("); break;
    case TAC_PARAM:      fprintf(stderr, "PARAM("); break;
    case TAC_RETURN:     fprintf(stderr, "RETURN("); break;
    case TAC_PRINT:      fprintf(stderr, "PRINT("); break;
    case TAC_LABEL:      fprintf(stderr, "LABEL("); break;
    case TAC_IF:         fprintf(stderr, "IF("); break;
    case TAC_JUMP:       fprintf(stderr, "JUMP("); break;
    case TAC_FUNCSTART:  fprintf(stderr, "FUNCSTART("); break;
    case TAC_FUNCEND:    fprintf(stderr, "FUNCEND("); break;
    case TAC_ARG:        fprintf(stderr, "ARG("); break;
    case TAC_ASSIGNVEC:  fprintf(stderr, "ASSIGNVEC("); break;
    case TAC_READ:       fprintf(stderr, "READ("); break;
    default: fprintf(stderr, "UNKNOWN TAC TYPE("); break;
    }

    fprintf(stderr, ",%s,%s,%s);\n", 
        (tac->res) ? tac->res->text : "0", 
        (tac->op1) ? tac->op1->text : "0", 
        (tac->op2) ? tac->op2->text : "0");
}

tac *tacGenerateCode(AST *node, HASH_NODE *currentLoopLabel)
{
    if (!node) {
        return NULL;
    }

    tac *sons[MAX_SONS];
    if (node->type == AST_WHILE)
    {
        currentLoopLabel = makeLabel();
    }

    for (int i = 0; i < MAX_SONS; i++) {
        sons[i] = tacGenerateCode(node->son[i], currentLoopLabel);
    }

    switch (node->type)
    {
    case AST_SYMBOL:
        return tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
    case AST_ADD:
        return createBinop(TAC_ADD, sons);
    case AST_SUB:
        return createBinop(TAC_SUB, sons);
    case AST_MUL:
        return createBinop(TAC_MUL, sons);
    case AST_DIV:
        return createBinop(TAC_DIV, sons);
    case AST_GTR:
        return createBinop(TAC_GTR, sons);
    case AST_LS:
        return createBinop(TAC_LS, sons);
    case AST_OPEQ:
        return createBinop(TAC_OPEQ, sons);
    case AST_OPGE:
        return createBinop(TAC_OPGE, sons);
    case AST_OPLE:
        return createBinop(TAC_OPLE, sons);
    case AST_OPDIF:
        return createBinop(TAC_OPDIF, sons);
    case AST_AND:
        return createBinop(TAC_AND, sons);
    case AST_OR:
        return createBinop(TAC_OR, sons);
    case AST_NEG:
        return createBinop(TAC_NEG, sons);
    
    case AST_ASSIGN:
        return tacJoin(sons[1], tacCreate(TAC_MOVE, node->son[0]->symbol, sons[1] ? sons[1]->res : 0, 0));
    
    case AST_ASSIGN_VECTOR:
            return tacJoin(sons[1], tacJoin(sons[2], tacCreate(TAC_ASSIGNVEC, node->son[0]->symbol, sons[1]->res, sons[2]->res)));
    
    case AST_VARDECL:
        return tacJoin(sons[0], tacCreate(TAC_MOVE, node->son[1]->symbol, sons[2] ? sons[2]->res : 0, 0));

    case AST_VECTORDECL:
        return tacCreate(TAC_MOVEVEC, node->son[1]->symbol, sons[2] ? sons[2]->res : makeTemp(), NULL);

    case AST_FUNCDECL:
        return createFunction(tacCreate(TAC_SYMBOL, node->symbol, 0, 0), sons[3], NULL);

    case AST_PARAM:
        return tacJoin(tacCreate(TAC_PARAM, node->son[1]->symbol, 0, 0), sons[2]);
    
    case AST_RETURN:
        return tacJoin(sons[0], tacCreate(TAC_RETURN, sons[0] ? sons[0]->res : 0, 0, 0));

    case AST_IFELSE:
        return createIf(sons);

    case AST_IF:
        return createIf(sons);

    case AST_WHILE:
        return createLoop(sons, currentLoopLabel);
        
   case AST_FUNCCALL:
         return tacJoin(sons[1], tacCreate(TAC_FUNCCALL, makeTemp(), node->son[0]->symbol, 0));

    case AST_ARG:
        return tacJoin(tacCreate(TAC_ARG, sons[0] ? sons[0]->res : 0, 0, 0), sons[1]);
    
    case AST_VECTOR:
        return tacCreate(TAC_VECTOR, node->son[0]->symbol, sons[1]->res, 0);

    case AST_ARG_LIST:
        return tacJoin(sons[0], sons[1]);

    case AST_PARAM_LIST:
        return tacJoin(sons[0], sons[1]);

    case AST_PRINT:
        return tacJoin(sons[0], tacCreate(TAC_PRINT, sons[0] ? sons[0]->res : 0, 0, 0));

    case AST_PRINT_EXPR:
        return tacJoin(sons[1], tacCreate(TAC_PRINT, sons[1]->res, 0, 0));

    case AST_READ:
        return tacJoin(sons[0], tacCreate(TAC_READ, node->son[1]->symbol, 0, 0));
        
    default:
        return tacJoin(tacJoin(tacJoin(sons[0], sons[1]), sons[2]), sons[3]);
    }
}


tac *createBinop(int type, tac *sons[])
{
    HASH_NODE *op1;
    HASH_NODE *op2;
    if (sons[0])
        op1 = sons[0]->res;
    else
        op1 = 0;
    if (sons[1])
        op2 = sons[1]->res;
    else
        op2 = 0;
    return tacJoin(sons[0], tacJoin(sons[1], tacCreate(type, makeTemp(), op1, op2)));
}

tac *createIf(tac *sons[])
{
    HASH_NODE *ifLabel = makeLabel();
    tac *ifTac = tacJoin(sons[0], tacCreate(TAC_IF, ifLabel, sons[0] ? sons[0]->res : 0, 0));
    tac *ifLabelTac = tacCreate(TAC_LABEL, ifLabel, 0, 0);

    if (sons[2])
    {
        HASH_NODE *elseLabel = makeLabel();
        tac *elseLabelTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);
        tac *elseJumpTac = tacCreate(TAC_JUMP, elseLabel, 0, 0);
        tac *ifElseTac = tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(ifTac, sons[1]), elseJumpTac), ifLabelTac), sons[2]), elseLabelTac);
        return ifElseTac;
    }
    else
    {
        return tacJoin(tacJoin(ifTac, sons[1]), ifLabelTac);
    }
}

tac *createLoop(tac *sons[], HASH_NODE *whileLabel)
{
    HASH_NODE *jumpLabel = makeLabel();

    tac *whileTac = tacCreate(TAC_IF, jumpLabel, sons[0] ? sons[0]->res : 0, 0);
    tac *whileLabelTac = tacCreate(TAC_LABEL, whileLabel, 0, 0);
    tac *jumpTac = tacCreate(TAC_JUMP, whileLabel, 0, 0);
    tac *jumpLabelTac = tacCreate(TAC_LABEL, jumpLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(whileLabelTac, sons[0]), whileTac), sons[1]), jumpTac), jumpLabelTac);
}

tac *createFunction(tac *symbol, tac *code, tac *rest)
{
    return tacJoin(tacJoin(tacJoin(tacCreate(TAC_FUNCSTART, symbol->res, 0, 0), code), tacCreate(TAC_FUNCEND, symbol->res, 0, 0)), rest);
}