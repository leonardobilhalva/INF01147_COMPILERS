%{
#include "tokens.h"
#include "hash.h"
int running = 1;
int lineNumber = 1;
%}

%x COMMENT

%%
"<="             {return OPERATOR_LE;}
">="             {return OPERATOR_GE;}
"=="             {return OPERATOR_EQ;}
"!="             {return OPERATOR_DIF;}

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

[0-9]+		     {hashInsert(yytext, LIT_INT); return LIT_INT;}
[a-zA-Z]+        {return LIT_CHAR;}

[-,;:()[]{}=+*/%<>&|~]   {return yytext[0];}

[ \n\t]          {return TOKEN_ERROR;}
"//".* 
"/*"            {BEGIN (COMMENT);}
. 

<COMMENT>"*/" { BEGIN (INITIAL); } 
<COMMENT>"\n"  {++lineNumber; }
<COMMENT>. 

%%

#include "hash.c"
#include "main.c"

int yywrap ()
{
running = 0;
return 1;
}