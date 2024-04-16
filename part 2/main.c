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

  hashInit();
  while (running)
  {
    tok = yylex();

    if (running == 0)
      break;
    switch (tok)
    {
    case KW_CHAR:
      printf("Found KW_CHAR\n");
      break;
    case KW_INT:
      printf("Found KW_INT\n");
      break;
    case KW_FLOAT:
      printf("Found KW_FLOAT\n");
      break;
    case KW_BOOL:
      printf("Found KW_BOOL\n");
      break;
    case KW_IF:
      printf("Found KW_IF\n");
      break;
    case KW_ELSE:
      printf("Found KW_ELSE\n");
      break;
    case KW_WHILE:
      printf("Found KW_WHILE\n");
      break;
    case KW_READ:
      printf("Found KW_READ\n");
      break;
    case KW_PRINT:
      printf("Found KW_PRINT\n");
      break;
    case KW_RETURN:
      printf("Found KW_RETURN\n");
      break;
    case OPERATOR_LE:
      printf("Found OPERATOR_LE\n");
      break;
    case OPERATOR_GE:
      printf("Found OPERATOR_GE\n");
      break;
    case OPERATOR_EQ:
      printf("Found OPERATOR_EQ\n");
      break;
    case OPERATOR_DIF:
      printf("Found OPERATOR_DIF\n");
      break;
    case TK_IDENTIFIER:
      printf("Found TK_IDENTIFIER\n");
      break;
    case LIT_INT:
      printf("Found LIT_INT\n");
      break;
    case LIT_CHAR:
      printf("Found LIT_CHAR\n");
      break;
    case LIT_REAL:
      printf("Found LIT_REAL\n");
      break;
    case LIT_FALSE:
      printf("Found LIT_FALSE\n");
      break;
    case LIT_TRUE:
      printf("Found LIT_TRUE\n");
      break;
    case LIT_STRING:
      printf("Found LIT_STRING\n");
      break;
    case TOKEN_ERROR:
      printf("Found token error\n");
      break;
    default:
      printf("Found %c\n", yytext[0]);
      break;
    }
  }
  hashPrint();
  printf("Main done! File has %d lines\n", lineNumber);
  exit(0);
}