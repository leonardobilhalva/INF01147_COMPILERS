// Leonardo Barros Bilhalva - 315768

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int tok;
  if (argc < 2)
  {
    printf(stderr, "Call: ./a.out file_name\n");
    exit(1);
  }
  yyin = fopen(argv[1], "r");
  if (yyin == 0)
  {
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(2);
  }

  hashInit();

  yyparse();

  hashPrint();
  // printf("Main done! File has %d lines\n", lineNumber);
  printf(stderr, "Compilation succesfull!\n");
  exit(0);
}