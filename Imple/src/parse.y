%code requires{
#include <string>
#include <vector>
#include <map>
class Scanner;
#include "Type.h"
}

%{
#include <iostream>
#include "Scanner.h"
#include <string>
#include <vector>
#include <map>

#include"parse.hpp"



using namespace std;

void error (const yy::location& loc,const std::string& st){
    //cout << "hey" << endl;
    if(loc.begin.line != loc.end.line)
        cerr <<"File \"" << *loc.begin.filename << "\" , line "
             << loc.begin.line << "-" << loc.end.line << " : " << st<< endl;
     else
         cerr <<"File \"" << *loc.begin.filename << "\" , line "
              << loc.begin.line << ", characters " << loc.begin.column
              << "-" << loc.end.column <<": " << st<< endl;
     exit(EXIT_FAILURE);
}

void yy::parser::error(const yy::location& loc,const std::string& st)
{
    ::error(loc,st); // calling global error function and not this one.
}

%}

// C++ declaration
%skeleton "lalr1.cc"
%language "c++"
%define api.value.type variant
%define lr.type ielr

// Token declaration
%token TYPE

//operator declaration
%token EQUAL COMMA BAR LP RP LTEMP RTEMP SEMICOLON

//typed tokens
%token  <std::string>   IDENT

%parse-param {Scanner& scan}
%parse-param {std::pair<std::string,Type*>& res}

%code{
    // declare the parser fonction to call :
#define yylex scan.yylex
 }

// Better error explanation than "syntax error"
%define parse.error verbose

 //Location tracking for errors
%locations

 //Location initialisation
%initial-action
{
    @$.initialize (scan.getName());
};

%type   <Type*> type
%type   <Type*> prodTypeElem
%type   <Type*> intType
%type   <std::vector<Type*> > prodType
%type   <std::vector<Type*> > sumType
%type   <Type*> sumTypeElem
%type   <std::pair<std::string,Type*>> typeDeclaration

//                      %precedence PRODCOMP
//                      %precedence BAR



%start program

%%

program:
        typeDeclaration {
            res = $1;
        }
    ;

typeDeclaration:
        TYPE IDENT EQUAL type SEMICOLON
        {$4->base_name($2); $$ = make_pair($2,$4);}
    ;

type:
        LP type RP {$$ = $2;}
    |
        IDENT {$$ = new BasicType($1,0);}
    |
        prodType {$$ = new ProdType($1);}
    |
        sumType {$$ = new SumType ($1);}
    ;

prodTypeElem:
        LP type RP {$$ = $2;}
    |
        IDENT {$$ = new BasicType($1,0);}
    ;

prodType:
        prodTypeElem prodTypeElem {$$ = {$1,$2};}
    |
    prodType prodTypeElem {$1.push_back($2); $$ = $1;}
    ;

sumType:
        sumTypeElem {$$ = {$1};}
    |
        sumType sumTypeElem {$1.push_back($2); $$ = $1;}
    ;

intType:
        LP type RP {$$ = $2;}
    |
        IDENT {$$ = new BasicType($1,0);}
    |
        prodType {$$ = new ProdType($1);}
    ;

sumTypeElem:
        BAR IDENT intType {$3->base_name($2); $$ = $3;}
    ;

%%


