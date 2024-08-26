%{
// Leonardo Barros Bilhalva - 315768
#include "hash.h"
#include "ast.h"
#include "y.tab.h"
int running = 1;
int lineNumber = 1;

int getLineNumber(){
    return lineNumber;
}
%}

%x COMMENT

%%

bool             {return KW_BOOL;}
char             {return KW_CHAR;}
int              {return KW_INT;}
float            {return KW_FLOAT;}
if               {return KW_IF;}
else             {return KW_ELSE;}
while            {return KW_WHILE;}
read             {return KW_READ;}
print            {return KW_PRINT;}
return           {return KW_RETURN;}

true             { yylval.symbol = hashInsert(yytext, LIT_TRUE); return LIT_TRUE; }
false            { yylval.symbol = hashInsert(yytext, LIT_FALSE); return LIT_FALSE; }

[-,;:()\[\]{}=+*/%<>&|~]            {return yytext[0];}

"<="                                {return OPERATOR_LE;}
">="                                {return OPERATOR_GE;}
"=="                                {return OPERATOR_EQ;}
"!="                                {return OPERATOR_DIF;}

[a-zA-Z_][a-zA-Z_0-9]*		        { yylval.symbol = hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER;}

[0-9]+						        { yylval.symbol = hashInsert(yytext, LIT_INT); return LIT_INT;}
'.'							        { yylval.symbol = hashInsert(yytext, LIT_CHAR); return LIT_CHAR;}
[0-9]*\.[0-9]+|[0-9]+\.[0-9]*       { yylval.symbol = hashInsert(yytext, LIT_REAL); return LIT_REAL;}
\"(\\.|[^"])*\"                     { yylval.symbol = hashInsert(yytext, LIT_STRING); return LIT_STRING;}

[ \t]            
"\n"             {lineNumber++;}
"//".* 
"/*"             {BEGIN (COMMENT);}
.                {return TOKEN_ERROR;}

<COMMENT>"*/"    {BEGIN (INITIAL);} 
<COMMENT>"\n"    {lineNumber++;}
<COMMENT>. 

%%

int yywrap (){
running = 0;
return 1;
}

void initMe(void){
	hashInit();
}

int isRunning(void){
    return running;
}
