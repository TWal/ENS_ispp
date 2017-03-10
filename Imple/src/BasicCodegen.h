
#ifndef BASICCODEGEN_H
#define BASICCODEGEN_H

#include "Type.h"

class BasicCodegen : public Codegen {
public:
    explicit BasicCodegen(std::ostream& out, const std::map<std::string, Type*>& defined)
        : Codegen(out, defined)
    { }
    void define(const std::vector<Type*>& ts);
    void declare(const std::vector<Type*>& ts);
    void codegen(const std::vector<Type*>& ts);
    void define_prod(const std::string& name, const std::vector<Type*>& types);
    void declare_prod(const std::string& name, const std::vector<Type*>& types);
    void gen_prod(const std::string& name, const std::vector<Type*>& types);
    std::string ref_prod(const std::string& name, const std::vector<Type*>& types) const;
    void define_sum(const std::string& name, const std::vector<Type*>& types);
    void declare_sum(const std::string& name, const std::vector<Type*>& types);
    void gen_sum(const std::string& name, const std::vector<Type*>& types);
    std::string ref_sum(const std::string& name, const std::vector<Type*>& types) const;
    void define_basic(const std::string& name);
    void declare_basic(const std::string& name);
    void gen_basic(const std::string& name);
    std::string ref_basic(const std::string& name) const;
};

#endif

