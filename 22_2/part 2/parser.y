%require "3.2"
%language "c++"

%code requires {
    // Leonardo Bilhalva - 315768 | Artur Turatti - 304740
    // explanation for parser interface https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison
    #include <string>
    #include "lexer.hh"
    extern int getLineNumber();
}

%define api.value.type {std::string}

%parse-param {Lexer &lexer}

%header

%code {
    #define yylex lexer.yylex
}

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
            | type identifier'['int']' 
            | type identifier'['int']' '=' vecParams
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

print : KW_PRINT listPrintArgs;

read : KW_READ identifier;

return : KW_RETURN expr;

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
        | expr
        |
        ;

nonEmptyFunctionCallParams: ',' expr nonEmptyFunctionCallParams
          |
          ;

identifier: TK_IDENTIFIER;

value: LIT_CHAR
    | int
    ;

int : LIT_INT;

string: LIT_STRING;

type: KW_CHAR
    | KW_INT
    ;

%%

using namespace std;

void yy::parser::error(const std::string &message)
{
    cerr << "Syntax error at line " << getLineNumber() << ": " << message << endl;
    exit(3);
}
