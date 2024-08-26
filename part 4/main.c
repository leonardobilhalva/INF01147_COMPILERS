// Leonardo Barros Bilhalva - 315768

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

extern FILE *yyin;
extern AST *getRoot();
int yyparse();
int getLineNumber();

extern int checkSemantic();

int main(int argc, char **argv)
{
    FILE *output;

    if (argc < 3)
    {
        fprintf(stderr, "Call: ./etapa3 file_name file_name\n");
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == 0)
    {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(2);
    }

    output = fopen(argv[2], "w+");
    if (output == 0)
    {
        fprintf(stderr, "Cannot open file %s\n", argv[2]);
        exit(2);
    }

    hashInit();

    fprintf(stderr, "Starting parser...\n");
    yyparse();
    fprintf(stderr, "Parser finished.\n");

    AST *root = getRoot();
    if (root == 0)
    {
        fprintf(stderr, "Root is null.\n");
        exit(3);
    }

    fprintf(stderr, "Checking Semantic...\n");
    checkSemantic();
    fprintf(stderr, "No Semantic Errors.\n");

    // fprintf(stderr, "Printing AST code to output file...\n");
    // astPrintCode(root, output);
    // fprintf(stderr, "AST code printed.\n");

    fclose(output);

    fprintf(stderr, "Compilation successful!\n");
    return 0;
}
