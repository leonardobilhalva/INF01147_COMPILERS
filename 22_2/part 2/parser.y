%{
#include <stdio.h>
#include <stdlib.h>
#include "mapContainer.h"

extern int yylex();
extern void yyerror(const char *message);
extern int getLineNumber();
%}

%token KW_CHAR
%token KW_INT
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_READ
%token KW_PRINT
%token KW_RETURN
%token TK_IDENTIFIER
%token LIT_INT
%token LIT_CHAR
%token LIT_STRING
%token TOKEN_ERROR

%left '&' '|' '~'
%left '<' '>' '='
%left '+' '-'
%left '*' '/'

%%

program: listGlobalDec;

listGlobalDec: varDec ';' listGlobalDec
            | functionDec listGlobalDec
            |
            ;

varDec: type identifier '=' value
            | type identifier '[' int ']'
            | type identifier '[' int ']' '=' vecParams
            ;

vecParams: value vecParams
        |
        ;

functionDec: type identifier '(' functionDecParams ')' block;

functionDecParams: type identifier nonEmptyFunctionDecParams
                 |
                 ;

nonEmptyFunctionDecParams: ',' type identifier nonEmptyFunctionDecParams
          |
          ;

block: '{' lcmd '}';

lcmd: cmd lcmd
    |
    ;

cmd: block
    | assign ';'
    | print ';'
    | read ';'
    | return ';'
    | flowControl
    | ';'
    ;

assign: identifier '=' expr
    | identifier '[' expr ']' '=' expr
    ;

print: KW_PRINT listPrintArgs;

read: KW_READ identifier;

return: KW_RETURN expr;

flowControl: KW_IF '(' expr ')' KW_THEN cmd
    | KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd
    | KW_WHILE '(' expr ')' cmd
    ;

listPrintArgs: string listPrintArgs
    | expr listPrintArgs
    | expr
    | string
    ;

expr: '(' expr ')'
    | identifier
    | identifier '[' expr ']'
    | value
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '>' expr
    | expr '<' expr
    | expr '&' expr
    | expr '|' expr
    | expr '=' expr
    | expr '~' expr
    | identifier '(' functionCallParams ')'
    ;

functionCallParams: expr nonEmptyFunctionCallParams
        |
        ;

nonEmptyFunctionCallParams: ',' expr nonEmptyFunctionCallParams
          |
          ;

identifier: TK_IDENTIFIER;

value: LIT_CHAR
    | int
    ;

int: LIT_INT;

string: LIT_STRING;

type: KW_CHAR
    | KW_INT
    ;

%%

void yyerror(const char *message) {
    fprintf(stderr, "Syntax error at line %d: %s\n", getLineNumber(), message);
}
