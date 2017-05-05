
#include <BasicCodegen.h>
#include <map>
#include <iostream>
using namespace std;
void BasicCodegen::declare(const FullType& ft){
    _out << "class " << ft.name << ";" << endl;
}
void BasicCodegen::define(const FullType& ft){
    _out << "class " << ft.name << "{" << endl;

    // ENUM
    _out << "\tenum {" <<endl;
    for(auto p : ft.content){
        _out <<"\t\t"<< p.first <<"_e," << endl;
    }
    _out << "\t};" <<endl;

    _out << "\tint type;" <<endl;


    // structs
    for(auto p : ft.content){
        _out << "\tstruct " << p.first << "_t {" << endl;
        for(size_t i = 0 ; i < p.second.size() ; ++i){
            auto lt = p.second[i];
            if(lt.pointedType) _out << "\t\t" << lt.name << "* m" << i << ";" <<endl;
            else _out << "\t\t" << lt.name << " m" << i << ";" << endl;
        }
        _out <<endl<< "\t};" << endl;

    }

    //union
    _out << "\tunion {" <<endl;
    for(auto p : ft.content){
        _out << "\t\t" << p.first << "_t " << p.first << ";" << endl;
    }
    _out << "\t};" <<endl;

    // private constructor
    _out << "\t" << ft.name << "(){}" << endl;
    _out << "public :" <<endl;

    //build
    for(auto p : ft.content){
        protoBuilder(_out,ft.name,p);
    }
    // match
    protoMatch(_out,ft);
    _out << "};" <<endl ;
}

void BasicCodegen::implement(const FullType& ft){
    //build
    for(auto p : ft.content){
        headBuilder(_out,ft.name,p);
        _out << "\t" << ft.name << "* res = new "<< ft.name << ";" <<endl;
        _out << "\tres->type = " << p.first <<"_e;" << endl;
        for(size_t i = 0 ; i < p.second.size() ; ++i){
            auto lt = p.second[i];
            _out << "\tres->" << p.first << ".m" << i << "= m" <<i <<";"<<endl;
            //if(lt.pointedType) _out << lt.name << "* m" << i;
            //else _out <<lt.name << " m" << i;
        }
        _out << "\treturn res;" <<endl;
        _out << "}" <<endl;
    }
    // match
    headMatch(_out,ft);

    _out << "\tswitch(type){" << endl;
    for(auto p : ft.content){
        _out << "\t\tcase " << ft.name << "::" << p.first << "_e:" << endl;
        _out << "\t\t\treturn f" << p.first << "(";
        for(size_t i = 0 ; i < p.second.size() ; ++i){
            if(i) _out << ", ";
            _out << p.first << ".m"<<i;
        }
        _out << ");" << endl;
        _out << "\t\t\tbreak;" << endl;
    }
    _out << "\t}" << endl;

    _out << "}" <<endl;
}

