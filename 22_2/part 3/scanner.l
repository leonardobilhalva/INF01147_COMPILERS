%{
// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
#include "y.tab.hpp"
#include "mapContainer.h"
#include "lexer.hh"
#include "y.tab.hpp"
#include <string>

#undef  YY_DECL
#define YY_DECL int Lexer::yylex(std::string *const yylval)

int running = 1;
int lineNumber = 1;    
%}
%option c++
%option yyclass="Lexer"

%x COMMENT

%%


char             {return yy::parser::token::KW_CHAR;}
int              {return yy::parser::token::KW_INT;}
if               {return yy::parser::token::KW_IF;}
then             {return yy::parser::token::KW_THEN;}
else             {return yy::parser::token::KW_ELSE;}
while            {return yy::parser::token::KW_WHILE;}
read             {return yy::parser::token::KW_READ;}
print            {return yy::parser::token::KW_PRINT;}
return           {return yy::parser::token::KW_RETURN;}

[-,;:()\[\]{}=+*/%<>&|~]            {return YYText()[0];}

[a-zA-Z0-9]*                        {insertSymbol(YYText(), yy::parser::token::TK_IDENTIFIER); return yy::parser::token::TK_IDENTIFIER;}
#[0-9]+                             {insertSymbol(YYText(), yy::parser::token::LIT_INT); return yy::parser::token::LIT_INT;}
'.'                                 {insertSymbol(YYText(), yy::parser::token::LIT_CHAR); return yy::parser::token::LIT_CHAR;}
\"(\\.|[^"])*\"                     {insertSymbol(YYText(), yy::parser::token::LIT_STRING); return yy::parser::token::LIT_STRING;}

[ \t]            
"\n"             {lineNumber++;}
"//".* 
"/**"             {BEGIN (COMMENT);}
.                {return  yy::parser::token::TOKEN_ERROR;}

<COMMENT>"**/"    {BEGIN (INITIAL);} 
<COMMENT>"\n"    {lineNumber++;}
<COMMENT>. 

%%

int yyFlexLexer::yywrap() {
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