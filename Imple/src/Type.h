#ifndef TYPE_H
#define TYPE_H

#include <cstddef>
#include <map>
#include <vector>
#include <string>
#include <iostream>

class Type {

public :
    virtual size_t getSize() = 0;
    virtual std::ostream& print(std::ostream& out) = 0;
};

class SumType : public Type {
    std::map<std::string,Type*> _types;
public :
    explicit SumType (const std::map<std::string,Type*>& types) : _types(types){}
    size_t getSize();
    std::ostream& print(std::ostream& out);
};

class ProdType : public Type {
    std::vector<Type*> _types;
public:
    explicit ProdType (const std::vector<Type*>& types) : _types(types){}
    size_t getSize();
    std::ostream& print(std::ostream& out);
};

class BasicType : public Type {
    std::string _name;
    size_t _size;
public :
    BasicType(std::string name,size_t size);
    inline size_t getSize(){return _size;}
    std::ostream& print(std::ostream& out);
};





#endif
