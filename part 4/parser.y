%{
	#include "hash.h"
	#include "ast.h"
	#include "semantic.h"
	#include <stdlib.h>

	int yylex();
	int yyerror();
	int getLineNumber();
	extern int verifySemantic();
	extern int semanticErrors;

	AST* root;
%}

%union
{
	HASH_NODE *symbol;
	AST *ast;
}

%token<symbol> KW_CHAR           
%token<symbol> KW_INT            
%token<symbol> KW_FLOAT          
%token<symbol> KW_BOOL           

%token KW_IF             
%token KW_ELSE           
%token KW_WHILE          
%token KW_READ           
%token KW_PRINT          
%token KW_RETURN         
        
%token OPERATOR_LE       
%token OPERATOR_GE       
%token OPERATOR_EQ       
%token OPERATOR_DIF      

%token<symbol> TK_IDENTIFIER     

%token<symbol> LIT_INT           
%token<symbol> LIT_CHAR  
%token<symbol> LIT_REAL          
%token<symbol> LIT_FALSE        
%token<symbol> LIT_TRUE          
%token<symbol> LIT_STRING        

%token TOKEN_ERROR

%type<ast> program
%type<ast> head
%type<ast> tail
%type<ast> globalVar
%type<ast> globalVector
%type<ast> loopVector
%type<ast> function
%type<ast> params
%type<ast> nonemptyParams
%type<ast> expr
%type<ast> value
%type<ast> functionCallArgs
%type<ast> nonemptyFunctionCallArgs
%type<ast> lcmd
%type<ast> block
%type<ast> cmd
%type<ast> assign
%type<ast> flowControl
%type<ast> read
%type<ast> print
%type<ast> return
%type<ast> type
%type<ast> int
%type<ast> string
%type<ast> identifier

%left TK_IDENTIFIER
%left '(' ')' '[' ']' '{' '}'
%left '&' '|' '~'
%left '<' '>' '='
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'

%%

program: head { root = $$; astPrint($1, 0); semanticErrors = verifySemantic($1);}
    ;

head: globalVar ';' tail { $$ = astCreate(AST_HEAD_GLOBAL_VAR, 0, $1, $3, 0, 0, getLineNumber()); }
    | function tail { $$ = astCreate(AST_HEAD, 0, $1, $2, 0, 0, getLineNumber()); }
    | { $$ = 0; }
    ;

tail: head { $$ = $1; }
    | { $$ = 0; }
    ;

globalVar: type identifier ':' value { $$ = astCreate(AST_VARDECL, 0, $1, $2, $4, 0, getLineNumber()); }
         | type identifier '[' int ']' globalVector { $$ = astCreate(AST_VECTORDECL, 0, $1, $2, $4, $6, getLineNumber()); }
         ;

globalVector: ':' value loopVector { $$ = astCreate(AST_GLOBAL_VECTOR, 0, $2, $3, 0, 0, getLineNumber()); }
    | { $$ = 0; }
    ;

loopVector: value loopVector { $$ = astCreate(AST_GLOBAL_VECTOR_LOOP, 0, $1, $2, 0, 0, getLineNumber()); }
    | { $$ = 0; }
    ;

function: type identifier '(' params ')' block { $$ = astCreate(AST_FUNCDECL, $2->symbol, $1, $2, $4, $6, getLineNumber()); }
    ;

params: nonemptyParams { $$ = $1; }
    | { $$ = 0; }
    ;

nonemptyParams: type identifier { $$ = astCreate(AST_PARAM, 0, $1, $2, 0, 0, getLineNumber()); }
    | type identifier ',' nonemptyParams { $$ = astCreate(AST_PARAM_LIST, 0, $1, $2, $4, 0, getLineNumber()); }
    ;

expr: '(' expr ')'               { $$ = astCreate(AST_PARENS, 0, $2, 0, 0, 0, getLineNumber()); }
    | TK_IDENTIFIER              { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
    | identifier '[' expr ']'    { $$ = astCreate(AST_VECTOR, 0, $1, $3, 0, 0, getLineNumber()); }
    | value                      { $$ = $1; }
    | expr '+' expr              { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '-' expr              { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '*' expr              { $$ = astCreate(AST_MUL, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '/' expr              { $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '>' expr              { $$ = astCreate(AST_GTR, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '<' expr              { $$ = astCreate(AST_LS, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '&' expr              { $$ = astCreate(AST_AND, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '|' expr              { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0, getLineNumber()); }
    | '~' expr                   { $$ = astCreate(AST_NEG, 0, $2, 0, 0, 0, getLineNumber()); }
    | expr OPERATOR_LE expr      { $$ = astCreate(AST_OPLE, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr OPERATOR_GE expr      { $$ = astCreate(AST_OPGE, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr OPERATOR_EQ expr      { $$ = astCreate(AST_OPEQ, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr OPERATOR_DIF expr     { $$ = astCreate(AST_OPDIF, 0, $1, $3, 0, 0, getLineNumber()); }
    | identifier '(' functionCallArgs ')' { $$ = astCreate(AST_FUNCCALL, 0, $1, $3, 0, 0, getLineNumber()); }
    ;

functionCallArgs: nonemptyFunctionCallArgs { $$ = $1; }
    | { $$ = 0; }
    ;

nonemptyFunctionCallArgs: expr ',' nonemptyFunctionCallArgs { $$ = astCreate(AST_ARG_LIST, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr { $$ = astCreate(AST_ARG, 0, $1, 0, 0, 0, getLineNumber()); }
    ;

lcmd: cmd lcmd { $$ = astCreate(AST_BLOCK_LIST, 0, $1, $2, 0, 0, getLineNumber()); }
    | { $$ = 0; }
    ;

block: '{' lcmd '}' { $$ = astCreate(AST_BLOCK, 0, $2, 0, 0, 0, getLineNumber()); }
    ;

cmd: block { $$ = $1; }
    | assign ';' { $$ = $1; }
    | flowControl { $$ = $1; }
    | read ';' { $$ = $1; }
    | print ';' { $$ = $1; }
    | return ';' { $$ = $1; }
    | ';' { $$ = 0; }
    ;

assign: identifier '=' expr { $$ = astCreate(AST_ASSIGN, 0, $1, $3, 0, 0, getLineNumber()); }
    | identifier '[' expr ']' '=' expr { $$ = astCreate(AST_ASSIGN_VECTOR, 0, $1, $3, $6, 0, getLineNumber()); }
    ;

flowControl: KW_IF '(' expr ')' cmd { $$ = astCreate(AST_IF, 0, $3, $5, 0, 0, getLineNumber()); }
    | KW_IF '(' expr ')' cmd KW_ELSE cmd { $$ = astCreate(AST_IFELSE, 0, $3, $5, $7, 0, getLineNumber()); }
    | KW_WHILE '(' expr ')' cmd { $$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0, getLineNumber()); }
    ;

read: KW_READ type identifier { $$ = astCreate(AST_READ, 0, $2, $3, 0, 0, getLineNumber()); }
    ;

print: KW_PRINT string { $$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0, getLineNumber()); }
    | KW_PRINT type expr { $$ = astCreate(AST_PRINT_EXPR, 0, $2, $3, 0, 0, getLineNumber()); }
    ;

return: KW_RETURN expr { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0, getLineNumber()); }
    ;

value: LIT_CHAR { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
    | LIT_INT   { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
    | LIT_REAL  { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
    | LIT_TRUE  { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
    | LIT_FALSE { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
    ;

type: KW_CHAR { $$ = astCreate(AST_KWCHAR, 0, 0, 0, 0, 0, getLineNumber()); }
    | KW_INT { $$ = astCreate(AST_KWINT, 0, 0, 0, 0, 0, getLineNumber()); }
    | KW_FLOAT { $$ = astCreate(AST_KWFLOAT, 0, 0, 0, 0, 0, getLineNumber()); }
    | KW_BOOL { $$ = astCreate(AST_KWBOOL, 0, 0, 0, 0, 0, getLineNumber()); }
    ;

int: 	LIT_INT							{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
	;

string: 	LIT_STRING							{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
	;

identifier: 	TK_IDENTIFIER							{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0, getLineNumber()); }
	;

%%

void checkSemantic(){
  if(semanticErrors > 0){
    fprintf(stderr, "%d Semantic Errors in Total.\n", semanticErrors);
    exit(4);
  }
}


AST* getRoot(){
	return root;
}

int yyerror(const char *s)
{
    fprintf(stderr, "Syntax error at line %d: %s\n", getLineNumber(), s);
    exit(3);
}
