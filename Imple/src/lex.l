

%{
#include<iostream>

#include"Scanner.h"

    using namespace std;
    using namespace yy;
    typedef yy::parser::token token;



#define yyerror error


// This code is executed at each action : update position in code.
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option c++
%option yyclass="Scanner"
%option yylineno
%option noyywrap

digit  [0-9]
letter [a-zA-Z]
label  {letter}({letter}|{digit}|_)*

%%
[ \t]
"\n" { assert(loc);loc->lines();}
"="  {return token::EQUAL;}
","  {return token::COMMA;}
"<"  {return token::LTEMP;}
">"  {return token::RTEMP;}
"("    {return token::LP;}
")"    {return token::RP;}
"|"  {return token::BAR;}
";"  {return token::SEMICOLON;}

"type" {return token::TYP;}

[[:alpha:]]([[:alnum:]])* { yyval->build(std::string(yytext)); return token::IDENT;}

. { error(*loc,"unknown token");}



%%
