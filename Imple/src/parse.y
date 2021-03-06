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



//using namespace std;

void error (const yy::location& loc,const std::string& st){
    //cout << "hey" << endl;
    if(loc.begin.line != loc.end.line)
        std::cerr <<"File \"" << *loc.begin.filename << "\" , line "
             << loc.begin.line << "-" << loc.end.line << " : " << st<< std::endl;
     else
         std::cerr <<"File \"" << *loc.begin.filename << "\" , line "
              << loc.begin.line << ", characters " << loc.begin.column
              << "-" << loc.end.column <<": " << st<< std::endl;
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
%token TYP

//operator declaration
%token EQUAL COMMA BAR LP RP LTEMP RTEMP SEMICOLON

//typed tokens
%token  <std::string>   IDENT

%parse-param {Scanner& scan}
%parse-param {std::vector<FullType>& res}

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

/*                      %type   <Type*> type
%type   <Type*> prodTypeElem
%type   <std::vector<Type*> > prodType
%type   <std::vector<Type*> > prodTypeSum
%type   <std::vector<Type*> > sumType
%type   <Type*> sumTypeElem
%type   <std::pair<std::string,Type*>> typeDeclaration*/

                      %type   <std::vector<FullType>> typeDecls
%type   <FullType> typeDecl
%type   <FullType> fulltype
%type   <std::pair<std::string,std::vector<LeafType>>> constructor
%type   <std::vector<LeafType>> list

//                      %precedence PRODCOMP
//                      %precedence BAR



%start program

%%

program:
        typeDecls {
            res= $1;
        }
    ;

typeDecls:
        typeDecl {
            $$ = std::vector<FullType>(1,$1);
        }
    |
        typeDecls typeDecl {
            $$ = $1; $$.push_back($2);
        }
    ;

typeDecl:
        TYP IDENT EQUAL fulltype SEMICOLON {
            $4.name = $2; $$ = $4;
        }

    ;

fulltype:
        constructor {
            $$.content.insert($1);
        }
    |
        fulltype constructor {
            $$ = $1; $$.content.insert($2);
        }
    ;

constructor:
        BAR IDENT list {
            $$ = make_pair($2,$3);
        }
    ;

list:
        {
            $$ = std::vector<LeafType>();
        }
    |
        list IDENT
        {
            $1.push_back(LeafType($2)); $$ = $1;
        }
    ;


/*typeDeclaration:
        TYP IDENT EQUAL type SEMICOLON
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

prodTypeSum:
        prodTypeElem {$$ = {$1};}
    |
        prodTypeSum prodTypeElem {$1.push_back($2); $$ = $1;}
    ;

sumType:
        sumTypeElem {$$ = {$1};}
    |
        sumType sumTypeElem {$1.push_back($2); $$ = $1;}
    ;

sumTypeElem:
        BAR IDENT prodTypeSum {$$ = new ProdType($3); $$->base_name($2);}
    ;
*/
%%


