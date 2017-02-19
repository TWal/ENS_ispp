#include "Type.h"
#include <algorithm>

using namespace std;

ostream& operator << (ostream&out,Type* type){
    return type->print(out);
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


BasicType::BasicType(std::string name,size_t size)
    :_name(name),_size(size){

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
