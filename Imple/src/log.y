%code requires{
#include <string>
#include <vector>
#include <map>
class LogScanner;
#include "Type.h"
#include "LogElem.h"
}

%{
#include <iostream>
#include "logScanner.h"
#include <string>
#include <vector>
#include <map>

#include "log.y.hpp"




using namespace std;
using namespace log;

void error (const log::location& loc,const std::string& st){
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

void log::parser::error(const log::location& loc,const std::string& st)
{
    ::error(loc,st); // calling global error function and not this one.
}

%}

// C++ declaration
%skeleton "lalr1.cc"
%language "c++"
%define api.value.type variant
%define lr.type ielr
%define api.namespace {log}

// Token declaration
%token AFFECT LP RP COMMA AS LB RB


//typed tokens
%token  <std::string>   IDENT
%token  <int>           OBJ

%parse-param {LogScanner& scan}
%parse-param {std::vector<LogElem>& res}

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

//                      %type   <Type*> type

//                      %precedence PRODCOMP
//                      %precedence BAR

%type   <std::vector<log::LogElem>> log
%type   <log::LogElem>         logEntry
%type   <std::vector<int>>     prodEntryList
%type   <log::TypeEntry*>      typeEntry
%type   <int>                  baseType


%start start

%%

start:
        log {res = move($1);}
    ;

log:
        logEntry {$$ = vector<LogElem>();}
    |
        log logEntry {$$ = $1; $$.push_back($2);}
    ;


logEntry:
        OBJ AFFECT typeEntry
        {$$ = LogElem{true,$1,$3};}
    |
        typeEntry AFFECT OBJ
        {$$ = LogElem{false,$3,$1};}
    ;

baseType:
        OBJ {$$ = $1;}
    |
        LB OBJ RB
        {$$ = - $2;}
    ;

prodEntryList:
        baseType {$$ = vector<int>{$1};}
    |
        prodEntryList COMMA baseType {$$ = $1, $$.push_back($3);}
    ;

typeEntry:
        IDENT LP prodEntryList RP
        {$$ = new ProdEntry($1,$3);}
    |
        IDENT AS IDENT OBJ
        {$$ = new SumEntry($1,$3,$4);}
    ;



%%



