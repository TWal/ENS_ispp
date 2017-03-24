#include <iostream>
#include "Scanner.h"
#include "../out/parse.hpp"
#include "Type.h"
#include "BasicCodegen.h"
#include "logScanner.h"
#include "LogElem.h"

using namespace std;

int main(){
    auto& input = cin;
    Scanner scan(&input,"test"); // load scanner
    pair<string,Type*> type;
    yy::parser parser(scan,type); //load parser
    parser.parse(); //parse
    type.second->print(cerr);
    cerr << endl;


    map<string,Type*> env;
    env[type.first] = type.second;
    BasicCodegen gen(cout, env);

    std::vector<Type*> decls(1, type.second);
    gen.define(decls);
    gen.declare(decls);
    gen.codegen(decls);
    return 0;
}
