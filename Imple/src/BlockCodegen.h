
#ifndef BLOCKCODEGEN_H
#define BLOCKCODEGEN_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "Type.h"

struct Block {
    size_t size;
    int filling_min; /* for 1000 */
    int filling_max; /* for 1000 */
};

class BlockCodegen : Codegen {
protected:
    std::vector<Block> _blocks;

public:
    explicit BlockCodegen(std::ostream& out,
            const std::map<std::string, Type*>& defined,
            const std::vector<Block>& blocks)
        : Codegen(out, defined), _blocks(blocks)
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
