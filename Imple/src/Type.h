#ifndef TYPE_H
#define TYPE_H

#include <cstddef>
#include <map>
#include <vector>
#include <string>
#include <iostream>

class Type {
protected:
    std::string _name;

public :
    explicit Type(const std::string& name) : _name(name) { }
    virtual size_t getSize() = 0;
    virtual std::ostream& print(std::ostream& out) = 0;
    virtual std::ostream& codegen(std::ostream& out) = 0;
    std::string name() const;
};

class SumType : public Type {
    std::map<std::string,Type*> _types;
public :
    explicit SumType (const std::string& name, const std::map<std::string,Type*>& types)
        : Type(name), _types(types) {}
    size_t getSize();
    std::ostream& print(std::ostream& out);
    std::ostream& codegen(std::ostream& out);
};

class ProdType : public Type {
    std::vector<Type*> _types;
public:
    explicit ProdType (const std::string& name, const std::vector<Type*>& types)
        : Type(name), _types(types) {}
    size_t getSize();
    std::ostream& print(std::ostream& out);
    std::ostream& codegen(std::ostream& out);
};

class BasicType : public Type {
    /* _name must be the name of a native c++ type */
    size_t _size;
public :
    explicit BasicType(const std::string& name,size_t size)
        : Type(name), _size(size) {}
    inline size_t getSize(){return _size;}
    std::ostream& print(std::ostream& out);
    std::ostream& codegen(std::ostream& out);
};





#endif
