#include <iostream>
#include "Scanner.h"
#include "../out/parse.hpp"
#include "Type.h"
#include "BlockCodegen.h"
#include "BasicCodegen.h"
#include "logScanner.h"
#include "LogElem.h"
//#include "CompCodeGen.h"

using namespace std;

int main(){
    auto& input = cin;
    Scanner scan(&input,"test"); // load scanner
    std::vector<FullType> types;
    yy::parser parser(scan,types); //load parser
    parser.parse(); //parse
    fillLink(types);

    Block blk = {
        .size = 64,
        .fmax = 64,
        .fmin = 0
    };
    vector<Block> blks = { blk, blk };

    BlockCodegen bcg(cout,cout,blks);
    bcg.declare(types[0]);
    bcg.define(types[0]);
    bcg.implement(types[0]);
    return 0;
}
