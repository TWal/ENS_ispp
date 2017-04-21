#include <iostream>
#include "Scanner.h"
#include "../out/parse.hpp"
#include "Type.h"
#include "BlockCodegen.h"
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

    Block b1, b2;
    b1.size = 64;
    b2.size = 64;
    b1.filling_min = b2.filling_min = 0;
    b1.filling_max = b2.filling_max = 64;
    vector<Block> blks({ b1, b2 });

    map<string,Type*> env;
    env[type.first] = type.second;
    // BasicCodegen gen(cout, env);
    BlockCodegen gen(cout, env, blks);

    std::vector<Type*> decls(1, type.second);
    gen.define(decls);
    gen.declare(decls);
    gen.codegen(decls);
    return 0;
}
