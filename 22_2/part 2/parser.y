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

program: ;

%%

void yy::parser::error(const std::string &message)
{
fprintf(stderr,"Syntax error at line %d\n", getLineNumber());
exit (3);
}