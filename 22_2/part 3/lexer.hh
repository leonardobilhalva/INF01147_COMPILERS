// Leonardo Bilhalva - 315768 | Artur Turatti - 304740
// ideia got from https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison

#pragma once

#include <string>
#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

class Lexer : public yyFlexLexer
{
public:
    int yylex(std::string *const yylval);
};
