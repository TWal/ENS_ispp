#include "Type.h"
#include <algorithm>

using namespace std;

ostream& operator << (ostream&out,const Type& type){
    return type.print(out);
}
std::map<std::string, Type*> Type::_defined;
void Type::define(const std::map<std::string, Type*>& d) {
    _defined = d;
}

size_t SumType::getSize(){
    size_t res = 0;
    for(auto p : _types){
        res = max (res,p->getSize()+1);
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

std::string SumType::name() const {
    return _name;
}
std::string ProdType::name() const {
    return _name;
}
std::string BasicType::name() const {
    if(_defined.find(_ref) != _defined.end())
	return _defined[_ref]->name();
    else
	return _ref;
}

void SumType::base_name(const std::string& nm) {
    _name = nm + "_t";
}
void ProdType::base_name(const std::string& nm) {
    _name = nm + "_t";
}
void BasicType::base_name(const std::string&) {
    /* Do nothing : base name is already set by construction */
}


std::ostream& SumType::print(std::ostream& out) const {
    out << _name << " (";
    for(auto p : _types){
        out << "| " << *p <<" ";
    }
    out << ")";
    return out;
}
std::ostream& ProdType::print(std::ostream& out) const {
    out << _name << " ";
    for(auto p : _types){
        out << *p <<" ";
    }
    return out;
}
std::ostream& BasicType::print(std::ostream& out) const {
    return out << name();
}

std::ostream& SumType::codegen(std::ostream& out) const{
    /* Dependencies */
    for(auto d : _types) d->codegen(out);

    /* Type definition */
    out << "struct " << _name << " {" << endl;
    out << "    size_t type;" << endl;
    out << "    union {" << endl;
    for(size_t i = 0; i < _types.size(); ++i) {
        out << "        " << _types[i]->name() << "* m" << i << ";" << endl;
    }
    out << "    };" << endl;
    out << endl;

    /* Building */
    for(size_t i = 0; i< _types.size(); ++i) {
        out << "    " << _name << "* build_" << _types[i]->name() << "(" << _types[i]->name() << "* b) {" << endl;
        out << "        " << _name << "* ret = new " << _name << ";" << endl;
        out << "        ret->type = " << i << ";" << endl;
        out << "        ret->m" << i << " = b;" << endl;
        out << "    }" << endl;
        out << endl;
    }

    /* Matching */
    out << "    template <typename T>" << endl;
    out << "    T match(" << _name << "* v";
    for(size_t i = 0; i < _types.size(); ++i) {
        out << ", const function<T(" << _types[i]->name() << ")>& f" << i;
    }
    out << ") {" << endl;
    out << "        switch(v->type) {" << endl;
    for(size_t i = 0; i < _types.size(); ++i) {
        out << "            case " << i << ":" << endl;
        out << "                f" << i << "(v->m" << i << ");" << endl;
        out << "                break;" << endl;
    }
    out << "        }" << endl;
    out << "    }" << endl;
    out << "};" << endl;
    out << endl;
    return out;
}
std::ostream& ProdType::codegen(std::ostream& out) const {
    /* Dependencies */
    for(auto d : _types) d->codegen(out);

    /* Type definition */
    out << "struct " << _name << " {" << endl;
    for(size_t i = 0; i < _types.size(); ++i) {
        out << "    " << _types[i]->name() << "* m" << i << ";" << endl;
    }

    /* Building */
    out << "    " << _name << "* build(";
    for(size_t i = 0; i < _types.size(); ++i) {
        out << _types[i]->name() << "* m" << i << "," << endl;
    }
    out << ") {" << endl;
    out << "    " << _name << "* bd = new " << _name << ";" << endl;
    for(size_t i = 0; i < _types.size(); ++i) {
        out << "        bd->m" << i << " = m" << i << ";" << endl;
    }
    out << "        return bd;" << endl;
    out << "    }" << endl;
    out << endl;

    /* Matching */
    out << "    template <typename T>" << endl;
    out << "    T match(" << _name << "* v, const std::function<T("
        << _types[0]->name() << "*";
    for(size_t i = 1; i < _types.size(); ++i) {
        out << ", " << _types[i]->name() << "_t*";
    }
    out << ")>& f) {" << endl;
    out << "    return f(";
    for(size_t i = 0; i < _types.size(); ++i) {
        out << (i == 0 ? "" : ", ") << "v->m" << i;
    }
    out << ");" << endl;
    out << "    }" << endl;

    out << "};" << endl;
    out << endl;
    return out;
}

std::ostream& BasicType::codegen(std::ostream& out) const {
    /* Nothing to do, it is a native c++/already defined type */
    return out;
}

