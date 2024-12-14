%{
// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include "y.tab.h"
#include "ast.hh"
#include "mapContainer.h"
#include <string>

int running = 1;
int lineNumber = 1;
%}

%x COMMENT

%%

char             { return KW_CHAR; }
int              { return KW_INT; }
if               { return KW_IF; }
then             { return KW_THEN; }
else             { return KW_ELSE; }
while            { return KW_WHILE; }
read             { return KW_READ; }
print            { return KW_PRINT; }
return           { return KW_RETURN; }

[-,;:()\[\]{}=+*/%<>&|~]            { return yytext[0]; }

[a-zA-Z0-9]*                        { yylval.symbol = insertSymbol(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }
#[0-9]+                             { yylval.symbol = insertSymbol(yytext, LIT_INT); return LIT_INT; }
'.'                                 { yylval.symbol = insertSymbol(yytext, LIT_CHAR); return LIT_CHAR; }
\"(\\.|[^"])*\"                     { yylval.symbol = insertSymbol(yytext, LIT_STRING); return LIT_STRING; }

[ \t]
"\n"             { lineNumber++; }
"//".*
"/**"             { BEGIN(COMMENT); }
.                 { return TOKEN_ERROR; }

<COMMENT>"**/"    { BEGIN(INITIAL); }
<COMMENT>"\n"     { lineNumber++; }
<COMMENT>.

%%

int yywrap() {
    running = 0;
    return 1;
}

int getLineNumber(void){
    return lineNumber;
}

void initMe(void){
}

int isRunning(void){
    return running;
}
