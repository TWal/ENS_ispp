#include <iostream>
#include "Scanner.h"
#include "parse.hpp"
#include "Type.h"
#include "BasicCodegen.h"

using namespace std;

int main(){
    /*
    auto& input = cin;
    Scanner scan(&input,"stderr"); // load scanner
    pair<string,Type*> type;
    yy::parser parser(scan,type); //load parser
    parser.parse(); //parse
    */

    Type *intt = new BasicType("int", sizeof(int));
    Type *btree = new BasicType("tree", 0);
    Type *leaf = new ProdType( { intt } );
    leaf->base_name("leaf");
    Type *node = new ProdType( { btree, btree } );
    node->base_name("node");
    Type *tree = new SumType(  { node, leaf } );
    tree->base_name("tree");


    map<string,Type*> env;
    env["tree"] = tree;
    BasicCodegen gen(cout, env);

    std::vector<Type*> decls(1, tree);
    gen.define(decls);
    gen.declare(decls);
    gen.codegen(decls);
    return 0;
}
