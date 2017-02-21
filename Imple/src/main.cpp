#include <iostream>
#include "Scanner.h"
#include "parse.hpp"

using namespace std;

int main(){
    auto& input = cin;
    Scanner scan(&input,"name of file for error"); // load scanner
    pair<string,Type*> res;
    yy::parser parser(scan,res); //load parser
    parser.parse(); //parse
}
