#include <iostream>
#include "Scanner.h"
#include "parse.hpp"

using namespace std;

int main(){
    auto& input = cin;
    Scanner scan(&input,"stderr"); // load scanner
    Type* type;
    yy::parser parser(scan,type); //load parser
    parser.parse(); //parse

    cout << "Parsed type :" << endl;
    cout << type << endl;
}
