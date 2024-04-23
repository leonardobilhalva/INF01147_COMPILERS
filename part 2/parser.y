// Leonardo Barros Bilhalva - 315768

%token KW_CHAR           
%token KW_INT            
%token KW_FLOAT          
%token KW_BOOL           

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

%token TK_IDENTIFIER     

%token LIT_INT           
%token LIT_CHAR  
%token LIT_REAL          
%token LIT_FALSE         
%token LIT_TRUE          
%token LIT_STRING        

%token TOKEN_ERROR

%left '&' '|' '~'
%left '<' '>' '='
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'

%%

program: head
    ;

head: globalVar ';' tail
    | function tail
    |
    ;

tail: head
    ;

globalVar: type TK_IDENTIFIER ':' value
	| type TK_IDENTIFIER '[' LIT_INT ']' globalVector
    ;

globalVector: ':' value loopVector
    |
    ;

loopVector: value loopVector
    |
    ;

function: type TK_IDENTIFIER '(' params ')' block
    ;

params: nonemptyParams
    |
    ;

nonemptyParams: type TK_IDENTIFIER
    | type TK_IDENTIFIER ',' nonemptyParams
    ;
    
expr: '(' expr ')'
    | TK_IDENTIFIER
    | TK_IDENTIFIER '[' expr ']'
    | value
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '>' expr
    | expr '<' expr
    | expr '&' expr
    | expr '|' expr
    | expr '~' expr
    | expr OPERATOR_LE expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_DIF expr
    | TK_IDENTIFIER '(' functionCallArgs ')'
    ;

functionCallArgs: nonemptyFunctionCallArgs
    |
    ;

nonemptyFunctionCallArgs: expr ',' nonemptyFunctionCallArgs
    | expr
    ;

lcmd: cmd lcmd
    |
    ;

block: '{' lcmd '}'
    ;

cmd: block
    | assign ';'
    | flowControl
    | read ';'
    | print ';'
    | return ';'
    | ';'
    ;

assign: TK_IDENTIFIER '=' expr
    | TK_IDENTIFIER '[' expr ']' '=' expr
    ;

flowControl: KW_IF '(' expr ')' cmd
    | KW_IF '(' expr ')' cmd KW_ELSE cmd
    | KW_WHILE '(' expr ')' cmd
    ;

read: KW_READ type TK_IDENTIFIER
    ;

print: KW_PRINT LIT_STRING
    | KW_PRINT type expr
    ;

return: KW_RETURN expr
    ;

value: LIT_CHAR
    | LIT_INT
    | LIT_REAL
    | LIT_TRUE
    | LIT_FALSE
    ;

type: KW_CHAR
    | KW_INT
    | KW_FLOAT
    | KW_BOOL
    ;

%%

int yyerror ()
{
fprintf(stderr,"Syntax error at line %d\n", getLineNumber());
exit (3);
}