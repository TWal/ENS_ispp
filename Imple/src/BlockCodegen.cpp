
#include <BlockCodegen.h>
#include <map>
#include <iostream>
using namespace std;
void BlockCodegen::declare(const FullType& ft){
    _out << "class " << ft.name << ";" << endl;
}
void BlockCodegen::define(const FullType& ft){
    // Blocks
    _out << "typedef charblock<" << _blocks[0].size << "> block0;";
    for(size_t i = 1; i < _blocks.size(); ++i) {
        _out << "typedef block<block" << i - 1 << ", "
            << _blocks[i].size << ", "
            << _blocks[i].fmax << ", "
            << _blocks[i].fmin << "> "
            << "block" << i << ";" << endl;
    }


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
    // free
    protoFree(_out,ft);
    _out << "};" <<endl ;
}

void BlockCodegen::implement(const FullType& ft){
    //build
    for(auto p : ft.content){
        headBuilder(_out,ft.name,p);
        // Block allocation
        int tp = -1;
        for(size_t i = 0; i < p.second.size(); ++i) {
            if(p.second[i].pointedType) {
                tp = i;
                break;
            }
        }
        _out << "\t" << ft.name << "* res = ("<< ft.name << "*)block_malloc<block"
            << _blocks.size() - 1 << ">((char*)";
        if(tp < 0) _out << "NULL";
        else       _out << "m" << tp;
        _out << ", sizeof(" << ft.name << "));" <<endl;

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

    // Free
    headFree(_out,ft);
    _out << "\tblock_free<block" << _blocks.size() - 1 << ">((char*)this, sizeof(this));" << endl;
    _out << "}" << endl;

}

