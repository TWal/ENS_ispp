#ifndef LOGSCANNER_H
#define LOGSCANNER_H

#if !defined(yyFlexLexerOnce)
#include<FlexLexer.h>
#endif

#include"../out/log/log.y.hpp"

//Scanning class
class LogScanner : public yyFlexLexer
{
public:
  //Scanner on the stream in
  LogScanner(std::istream* in,const std::string& filename ) : yyFlexLexer(in),name(filename) {}

  //read a token and store its value in yyval
  virtual int yylex (log::parser::semantic_type* yyval, log::parser::location_type*loc);

  std::string * getName()
  {
	return &name;
  }


private :

  std::string name;
};
// YY_DECL contain the signature of the function where flex puts its code
#undef YY_DECL
#define YY_DECL int LogScanner::yylex (log::parser::semantic_type* yyval, log::parser::location_type*loc)


//error function (implemented in asm.y)
void error (const log::location& loc,const std::string& st);

#endif
