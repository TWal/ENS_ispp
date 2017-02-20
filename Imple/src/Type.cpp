#include "Type.h"
#include <algorithm>

using namespace std;

ostream& operator << (ostream&out,Type* type){
    return type->print(out);
}
std::string Type::name() const {
    return _name;
}

size_t SumType::getSize(){
    size_t res = 0;
    for(auto p : _types){
        res = max (res,p.second->getSize()+1);
    }
    return res;
}

size_t ProdType::getSize(){
    size_t res = 0;
    for(auto p : _types){
        res += p->getSize()+1;
    }
    return res;
}


std::ostream& SumType::print(std::ostream& out){
    out << "(";
    for(auto p : _types){
        out << "| " << p.first << " " << p.second <<" ";
    }
    out << ")";
    return out;
}
std::ostream& ProdType::print(std::ostream& out){
    for(auto p : _types){
        out <<  p <<" ";
    }
    return out;
}
std::ostream& BasicType::print(std::ostream& out){
    return out << _name;
}

std::ostream& SumType::codegen(std::ostream& out){
    /* Type definition */
}
std::ostream& ProdType::codegen(std::ostream& out){
    /* Type definition */
    out << "struct " << _name << "_t {" << endl;
    for(size_t i = 0; i < _types.size(); ++i) {
        out << "    " << _types[i]->name() << "_t* m" << i << ";" << endl;
    }
    out << "};" << endl;

    /* Building */
    out << _name << "_t* build_" << _name << "(";
    for(size_t i = 0; i < _types.size(); ++i) {
        out << _types[i]->name() << "_t* m" << i << "," << endl;
    }
    out << ") {" << endl;
    out << "    " << _name << "_t* bd = new " << _name << "_t;" << endl;
    for(size_t i = 0; i < _types.size(); ++i) {
        out << "    bd->m" << i << " = m" << i << ";" << endl;
    }
    out << "    return bd;" << endl;
    out << "}" << endl;
    out << endl;

    /* Matching */
    out << "template <typename T>" << endl;
    out << "T match_" << _name << "(" << _name << "_t* v, const std::function<T("
        << _types[0]->name() << "_t*";
    for(size_t i = 1; i < _types.size(); ++i) {
        out << ", " << _types[i]->name() << "_t*";
    }
    out << ")>& f) {" << endl;
    out << "    return f(";
    for(size_t i = 0; i < _types.size(); ++i) {
        out << (i == 0 ? "" : ", ") << "v->m" << i;
    }
    out << ");" << endl;
    out << "}" << endl;
    return out;
}

std::ostream& BasicType::codegen(std::ostream& out){
    /* Type declaration */
    /*
    out << "typedef " << _name << " " << _name << "_t;" << endl;
    out << endl;
    */
    /* Building */
    out << _name << "_t* build_" << _name << "(" << _name << " v) {" << endl;
    out << "    " << _name << "_t bd = new " << _name << "_t(v);" << endl;
    out << "    " << "retun bd;" << endl;
    out << "}" << endl;
    out << endl;
    /* Matching */
    out << "template <typename T>" << endl;
    out << "T match_" << _name << "(" << _name << "_t* v, const std::function<T(" << _name << ")>& f) {" << endl;
    out << "    return f(*v);" << endl;
    out << "}" << endl;
    return out;
}

