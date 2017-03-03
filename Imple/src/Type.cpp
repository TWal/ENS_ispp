#include "Type.h"
#include <algorithm>

using namespace std;

ostream& operator << (ostream&out,const Type& type){
    return type.print(out);
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

void SumType::base_name(const std::string& nm) {
    _name = nm;
}
void ProdType::base_name(const std::string& nm) {
    _name = nm;
}
void BasicType::base_name(const std::string&) {
    /* Do nothing : base name is already set by construction */
}
string SumType::base_name() const {
    return _name;
}
string ProdType::base_name() const {
    return _name;
}
string BasicType::base_name() const {
    return _ref;
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
    return out << _ref;
}

void SumType::codegen(Codegen* gen) const{
    gen->gen_sum(_name, _types);
}
void ProdType::codegen(Codegen* gen) const {
    gen->gen_prod(_name, _types);
}
void BasicType::codegen(Codegen* gen) const {
    gen->gen_basic(_ref);
}
    
void SumType::declare(Codegen* gen) const {
    gen->declare_sum(_name, _types);
}
void ProdType::declare(Codegen* gen) const {
    gen->declare_prod(_name, _types);
}
void BasicType::declare(Codegen* gen) const {
    gen->declare_basic(_ref);
}

std::string SumType::ref(const Codegen* gen) const {
    return gen->ref_sum(_name, _types);
}
std::string ProdType::ref(const Codegen* gen) const {
    return gen->ref_prod(_name, _types);
}
std::string BasicType::ref(const Codegen* gen) const {
    return gen->ref_basic(_ref);
}
