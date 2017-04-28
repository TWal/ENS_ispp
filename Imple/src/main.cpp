#include <iostream>
#include "Scanner.h"
#include "../out/parse.hpp"
#include "Type.h"
//#include "BlockCodegen.h"
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

    BasicCodegen bcg(cout,cout);
    bcg.declare(types[0]);
    bcg.define(types[0]);
    bcg.implement(types[0]);
    return 0;
}
