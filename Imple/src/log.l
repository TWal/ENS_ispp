

%{
#include<iostream>

#include"logScanner.h"

    using namespace std;
    using namespace log;
    typedef log::parser::token token;



#define logerror error


// This code is executed at each action : update position in code.
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option c++
%option yyclass="LogScanner"
%option yylineno
%option noyywrap
%option prefix="log"


digit  [0-9]
letter [a-zA-Z]
label  {letter}({letter}|{digit}|_)*

%%
[ \t]
"\n" {assert(loc);loc->lines();}
"<"  {return token::AFFECT;}
"("  {return token::LP;}
")"  {return token::RP;}
"["  {return token::LB;}
"]"  {return token::RB;}
","  {return token::COMMA;}
"as" {return token::AS;}

[[:alpha:]]([[:alnum:]])* { yyval->build(std::string(yytext)); return token::IDENT;}
[[:digit:]]* {yyval->build(atoi(yytext)); return token::OBJ;}

. { error(*loc,"unknown token");}



%%
