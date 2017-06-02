#include <iostream>
#include "Scanner.h"
#include "../out/parse.hpp"
#include "Type.h"
#include "BlockCodegen.h"
#include "BasicCodegen.h"
#include "logScanner.h"
#include "LogElem.h"
#include "CompCodeGen.h"
#include <fstream>

using namespace std;

int main(int argc, char** argv){
    fstream f;
    if(argc >= 2) f.open(argv[1],ios_base::in);

    auto& input = argc >= 2 ? f : cin;
    Scanner scan(&input,"test"); // load scanner
    std::vector<FullType> types;
    yy::parser parser(scan,types); //load parser
    parser.parse(); //parse
    fillLink(types);

/*    Block blk = {
        .size = 64,
        .fmax = 64,
        .fmin = 0
    };
    vector<Block> blks = { blk, blk };*/
    

    //BlockCodegen bcg(cout,cout,blks);
    CompCodegen bcg(cout,cout);
    bcg.header();
    bcg.declare(types[0]);
    bcg.define(types[0]);
    bcg.implement(types[0]);
    return 0;
}
