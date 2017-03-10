#ifndef TYPE_H
#define TYPE_H

#include <cstddef>
#include <map>
#include <vector>
#include <string>
#include <iostream>

class Type;

class Codegen {
protected:
    std::ostream& _out;
    std::map<std::string, Type*> _defined;

public:
    explicit Codegen(std::ostream& out, const std::map<std::string, Type*>& defined)
        : _out(out), _defined(defined)
    { }
    virtual void define(const std::vector<Type*>& ts) = 0;
    virtual void declare(const std::vector<Type*>& ts) = 0;
    virtual void codegen(const std::vector<Type*>& ts) = 0;

    virtual void define_prod(const std::string& name, const std::vector<Type*>& types) = 0;
    virtual void declare_prod(const std::string& name, const std::vector<Type*>& types) = 0;
    virtual void gen_prod(const std::string& name, const std::vector<Type*>& types) = 0;
    virtual std::string ref_prod(const std::string& name, const std::vector<Type*>& types) const = 0;

    virtual void define_sum(const std::string& name, const std::vector<Type*>& types) = 0;
    virtual void declare_sum(const std::string& name, const std::vector<Type*>& types) = 0;
    virtual void gen_sum(const std::string& name, const std::vector<Type*>& types) = 0;
    virtual std::string ref_sum(const std::string& name, const std::vector<Type*>& types) const = 0;

    virtual void define_basic(const std::string& name) = 0;
    virtual void declare_basic(const std::string& name) = 0;
    virtual void gen_basic(const std::string& name) = 0;
    virtual std::string ref_basic(const std::string& name) const = 0;
};

class Type {
public :
    virtual size_t getSize() = 0;
    virtual std::ostream& print(std::ostream& out) const = 0;
    virtual void codegen(Codegen* gen) const = 0;
    virtual void declare(Codegen* gen) const = 0;
    virtual void define(Codegen* gen) const = 0;
    virtual std::string ref(const Codegen* gen) const = 0;
    virtual void base_name(const std::string&) = 0;
    virtual std::string base_name() const = 0;
    virtual bool is_native(const std::map<std::string, Type*>& defined) const = 0;
};

class SumType : public Type {
    std::vector<Type*> _types;
    std::string _name;

public :
    explicit SumType (const std::vector<Type*>& types)
        : _types(types) {}
    size_t getSize();
    std::ostream& print(std::ostream& out) const;
    void codegen(Codegen* gen) const;
    void declare(Codegen* gen) const;
    void define(Codegen* gen) const;
    std::string ref(const Codegen* gen) const;
    void base_name(const std::string&);
    std::string base_name() const;
    bool is_native(const std::map<std::string, Type*>& defined) const;
};

class ProdType : public Type {
    std::vector<Type*> _types;
    std::string _name;
public:
    explicit ProdType (const std::vector<Type*>& types)
        : _types(types) {}
    size_t getSize();
    std::ostream& print(std::ostream& out) const;
    void codegen(Codegen* gen) const;
    void declare(Codegen* gen) const;
    void define(Codegen* gen) const;
    std::string ref(const Codegen* gen) const;
    void base_name(const std::string&);
    std::string base_name() const;
    bool is_native(const std::map<std::string, Type*>& defined) const;
};

class BasicType : public Type {
    std::string _ref;
    size_t _size;
public :
    explicit BasicType(const std::string& ref,size_t size)
        : _ref(ref), _size(size) {}
    inline size_t getSize(){return _size;}
    std::ostream& print(std::ostream& out) const;
    void codegen(Codegen* gen) const;
    void declare(Codegen* gen) const;
    void define(Codegen* gen) const;
    std::string ref(const Codegen* gen) const;
    void base_name(const std::string&);
    std::string base_name() const;
    bool is_native(const std::map<std::string, Type*>& defined) const;
};

std::ostream& operator << (std::ostream& out,const Type& type);

#endif

