// Leonardo Barros Bilhalva - 315768

#include <stdio.h>
#include <stdlib.h>

// // lex.yy.h
// int yylex();
// extern char *yytext;
// extern FILE *yyin;

// int isRunning(void);
// void initMe(void);

int main(int argc, char **argv)
{
  int tok;
  if (argc < 2)
  {
    printf(stderr, "Call: ./a.out file_name\n");
    exit(1);
  }
  yyin = fopen(argv[1], "I");

  hashInit();
  while (running)
  {
    tok = yylex();
    if (running == 0)
      break;
    switch (tok)
    {
    case KW_INT:
      printf("Encontrei KW_INT\n");
      break;
    case LIT_INT:
      printf("Encontrei LIT_INT\n");
      break;
    case TOKEN_ERROR:
      printf("unexpected token %d\n", tok);
      break;
    default:
      printf("Encontrei %c\n", yytext[0]);
      break;
    }
  }
  printf("Main done! File has %d lines\n", lineNumber);
  exit(0);
}