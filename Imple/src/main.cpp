#include <iostream>
#include "Scanner.h"
#include "parse.hpp"

using namespace std;

int main(){
    auto& input = cin;
    Scanner scan(&input,"stderr"); // load scanner
    pair<string,Type*> type;
    yy::parser parser(scan,type); //load parser
    parser.parse(); //parse

    map<string, Type*> defined;
    defined.insert(type);

    cout << "Parsed type :" << endl;
    cout << type.second << endl;
}
