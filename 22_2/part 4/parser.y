%{
#include <stdio.h>
#include <stdlib.h>
#include "mapContainer.h"
#include "ast.hh"
#include "semantic.hh"

extern AST* root;
extern int yylex();
extern void yyerror(const char *message);
extern int getLineNumber();
extern int verifySemantic();
extern int semanticErrors;
%}

%code requires {
#include "ast.hh"
}

%union {
    Symbol* symbol;
    AST* ast;
}

%token<symbol> KW_CHAR
%token<symbol> KW_INT
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_READ
%token KW_PRINT
%token KW_RETURN
%token<symbol> TK_IDENTIFIER
%token<symbol> LIT_INT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING
%token TOKEN_ERROR

%type<ast> program
%type<ast> listGlobalDec
%type<ast> varDec
%type<ast> functionDec
%type<ast> functionDecParams
%type<ast> nonEmptyFunctionDecParams
%type<ast> block
%type<ast> lcmd
%type<ast> cmd
%type<ast> assign
%type<ast> print
%type<ast> read
%type<ast> return
%type<ast> flowControl
%type<ast> listPrintArgs
%type<ast> expr
%type<ast> functionCallParams
%type<ast> nonEmptyFunctionCallParams
%type<ast> identifier
%type<ast> value
%type<ast> int
%type<ast> string
%type<ast> type
%type<ast> vecParams

%left '&' '|' '~'
%left '<' '>' '='
%left '+' '-'
%left '*' '/'

%%

program: listGlobalDec { $$ = new AST(AST_PROGRAM, { $1 }, nullptr, getLineNumber()); root = $$; semanticErrors = verifySemantic($1); }
;

listGlobalDec: varDec ';' listGlobalDec { $$ = new AST(AST_LIST_GLOBAL_VAR_DEC, { $1, $3 }, nullptr, getLineNumber()); }
             | functionDec listGlobalDec { $$ = new AST(AST_LIST_GLOBAL_FUNC_DEC, { $1, $2 }, nullptr, getLineNumber()); }
             | { $$ = 0; }
;

varDec: type identifier '=' value { $$ = new AST(AST_VAR_DEC, { $1, $2, $4 }, nullptr, getLineNumber()); }
      | type identifier '[' int ']' { $$ = new AST(AST_VEC_NULL_DEC, { $1, $2, $4 }, nullptr, getLineNumber()); }
      | type identifier '[' int ']' '=' vecParams { $$ = new AST(AST_VEC_DEC, { $1, $2, $4, $7 }, nullptr, getLineNumber()); }
;

vecParams: value vecParams { $$ = new AST(AST_VEC_PARAMS, { $1, $2 }, nullptr, getLineNumber()); }
         | { $$ = 0; }
;

functionDec: type identifier '(' functionDecParams ')' block { $$ = new AST(AST_FUNCTION_DEC, { $1, $2, $4, $6 }, nullptr, getLineNumber()); }
;

functionDecParams: type identifier nonEmptyFunctionDecParams { $$ = new AST(AST_FUNCTION_DEC_PARAMS, { $1, $2, $3 }, nullptr, getLineNumber()); }
                  | { $$ = 0; }
;

nonEmptyFunctionDecParams: ',' type identifier nonEmptyFunctionDecParams { $$ = new AST(AST_NONEMPTY_FUNCTION_DEC_PARAMS, { $2, $3, $4 }, nullptr, getLineNumber()); }
                          | { $$ = 0; }
;

block: '{' lcmd '}' { $$ = new AST(AST_BLOCK, { $2 }, nullptr, getLineNumber()); }
;

lcmd: cmd lcmd { $$ = new AST(AST_LCMD, { $1, $2 }, nullptr, getLineNumber()); }
    | { $$ = 0; }
;

cmd: block { $$ = $1; }
   | assign ';' { $$ = $1; }
   | print ';' { $$ = $1; }
   | read ';' { $$ = $1; }
   | return ';' { $$ = $1; }
   | flowControl { $$ = $1; }
   | ';' { $$ = 0; }
;

assign: identifier '=' expr { $$ = new AST(AST_ASSIGN, { $1, $3 }, nullptr, getLineNumber()); }
      | identifier '[' expr ']' '=' expr { $$ = new AST(AST_ASSIGN_VEC, { $1, $3, $6 }, nullptr, getLineNumber()); }
;

print: KW_PRINT listPrintArgs { $$ = new AST(AST_PRINT, { $2 }, nullptr, getLineNumber()); }
;

read: KW_READ identifier { $$ = new AST(AST_READ, { $2 }, nullptr, getLineNumber()); }
;

return: KW_RETURN expr { $$ = new AST(AST_RETURN, { $2 }, nullptr, getLineNumber()); }
;

flowControl: KW_IF '(' expr ')' KW_THEN cmd { $$ = new AST(AST_IF, { $3, $6 }, nullptr, getLineNumber()); }
           | KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd { $$ = new AST(AST_IFELSE, { $3, $6, $8 }, nullptr, getLineNumber()); }
           | KW_WHILE '(' expr ')' cmd { $$ = new AST(AST_WHILE, { $3, $5 }, nullptr, getLineNumber()); }
;

listPrintArgs: string listPrintArgs { $$ = new AST(AST_NONEMPTY_LIST_PRINT_ARGS_STRING, { $1, $2 }, nullptr, getLineNumber()); }
             | expr listPrintArgs { $$ = new AST(AST_NONEMPTY_LIST_PRINT_ARGS_EXPR, { $1, $2 }, nullptr, getLineNumber()); }
             | expr { $$ = new AST(AST_LIST_PRINT_ARGS_STRING, { $1 }, nullptr, getLineNumber()); }
             | string { $$ = new AST(AST_LIST_PRINT_ARGS_EXPR, { $1 }, nullptr, getLineNumber()); }
;

expr: '(' expr ')' { $$ = new AST(AST_PARENS, { $2 }, nullptr, getLineNumber()); }
    | identifier { $$ = $1; }
    | identifier '[' expr ']' { $$ = new AST(AST_VEC_CALL, { $1, $3 }, nullptr, getLineNumber()); }
    | value { $$ = $1; }
    | expr '+' expr { $$ = new AST(AST_ADD, { $1, $3 }, nullptr, getLineNumber()); }
    | expr '-' expr { $$ = new AST(AST_SUB, { $1, $3 }, nullptr, getLineNumber()); }
    | expr '*' expr { $$ = new AST(AST_MUL, { $1, $3 }, nullptr, getLineNumber()); }
    | expr '/' expr { $$ = new AST(AST_DIV, { $1, $3 }, nullptr, getLineNumber()); }
    | expr '>' expr { $$ = new AST(AST_GTR, { $1, $3 }, nullptr, getLineNumber()); }
    | expr '<' expr { $$ = new AST(AST_LSS, { $1, $3 }, nullptr, getLineNumber()); }
    | expr '&' expr { $$ = new AST(AST_AND, { $1, $3 }, nullptr, getLineNumber()); }
    | expr '|' expr { $$ = new AST(AST_OR, { $1, $3 }, nullptr, getLineNumber()); }
    | expr '=' expr { $$ = new AST(AST_EQ, { $1, $3 }, nullptr, getLineNumber()); }
    | '~' expr { $$ = new AST(AST_NEG, { $2 }, nullptr, getLineNumber()); }
    | identifier '(' functionCallParams ')' { $$ = new AST(AST_FUNC_CALL, { $1, $3 }, nullptr, getLineNumber()); }
;

functionCallParams: expr nonEmptyFunctionCallParams { $$ = new AST(AST_FUNC_CALL_PARAMS, { $1, $2 }, nullptr, getLineNumber()); }
                  | { $$ = 0; }
;

nonEmptyFunctionCallParams: ',' expr nonEmptyFunctionCallParams { $$ = new AST(AST_NONEMPTY_FUNC_CALL_PARAMS, { $2, $3 }, nullptr, getLineNumber()); }
                          | { $$ = 0; }
;

identifier: TK_IDENTIFIER { $$ = new AST(AST_SYMBOL, {}, $1, getLineNumber()); }
;

value: LIT_CHAR { $$ = new AST(AST_SYMBOL, {}, $1, getLineNumber()); }
     | int { $$ = $1; }
;

int: LIT_INT { $$ = new AST(AST_SYMBOL, {}, $1, getLineNumber()); }
;

string: LIT_STRING { $$ = new AST(AST_SYMBOL, {}, $1, getLineNumber()); }
;

type: KW_CHAR { $$ = new AST(AST_KW_CHAR, {}, $1, getLineNumber()); }
    | KW_INT { $$ = new AST(AST_KW_INT, {}, $1, getLineNumber()); }
;

%%

AST* getRoot() { return root; }

void yyerror(const char *message) {
    fprintf(stderr, "Syntax error at line %d: %s\n", getLineNumber(), message);
}

void checkSemantic(){
  if(semanticErrors > 0){
    fprintf(stderr, "%d Semantic Errors in Total.\n", semanticErrors);
    exit(4);
  }
}
