#ifndef TYPE_H
#define TYPE_H

#include <cstddef>
#include <map>
#include <vector>
#include <string>
#include <iostream>

class Type {
protected:
    static std::map<std::string, Type*> _defined;

public :
    static void define(const std::map<std::string, Type*>& d);
    virtual size_t getSize() = 0;
    virtual std::ostream& print(std::ostream& out) const = 0;
    virtual std::ostream& codegen(std::ostream& out) const = 0;
    virtual std::string name() const = 0;
    virtual void base_name(const std::string&) = 0;
    virtual std::string base_name() const = 0;
};

class SumType : public Type {
    std::vector<Type*> _types;
    std::string _name;

public :
    explicit SumType (const std::vector<Type*>& types)
        : _types(types) {}
    size_t getSize();
    std::ostream& print(std::ostream& out) const;
    std::ostream& codegen(std::ostream& out) const;
    std::string name() const;
    void base_name(const std::string&);
    std::string base_name() const;
};

class ProdType : public Type {
    std::vector<Type*> _types;
    std::string _name;
public:
    explicit ProdType (const std::vector<Type*>& types)
        : _types(types) {}
    size_t getSize();
    std::ostream& print(std::ostream& out) const;
    std::ostream& codegen(std::ostream& out) const;
    std::string name() const;
    void base_name(const std::string&);
    std::string base_name() const;
};

class BasicType : public Type {
    std::string _ref;
    size_t _size;
public :
    explicit BasicType(const std::string& ref,size_t size)
        : _ref(ref), _size(size) {}
    inline size_t getSize(){return _size;}
    std::ostream& print(std::ostream& out) const;
    std::ostream& codegen(std::ostream& out) const;
    std::string name() const;
    void base_name(const std::string&);
    std::string base_name() const;
};

std::ostream& operator << (std::ostream& out,const Type& type);




#endif
