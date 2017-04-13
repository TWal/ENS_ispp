
#include "BlockCodegen.h"
#include <iostream>
#include <sstream>
using namespace std;

void BlockCodegen::define(const std::vector<Type*>& ts) {
    _out << "#include \"Blocks.h\"" << endl;
    _out << "typedef charblock<" << _blocks[0].size << "> block0;" << endl;
    for(size_t i = 1; i < _blocks.size(); ++i) {
        _out << "typedef block<block" << i - 1 << ", "
            << _blocks[i].size << ", "
            << _blocks[i].filling_max << ", "
            << _blocks[i].filling_min
            << "> block" << i << ";" << endl;
    }
    _out << "extern block0* first_block;" << endl;
    for(auto t: ts) t->define(this);
}
void BlockCodegen::declare(const std::vector<Type*>& ts) {
    for(auto t: ts) t->declare(this);
}
void BlockCodegen::codegen(const std::vector<Type*>& ts) {
    _out << "block0* first_block;" << endl;

    _out << "void init_blocks() {" << endl;
    /* Alloc a block of size zero near nothing */
    _out << "    first_block = (block0*)block_init<block"
        << _blocks.size() - 1 << ">();" << endl;
    /* It will point after the end of the allocated block, which is what we want
     * since we will use it as the initial next_to.
     */
    _out << "}" << endl;
    _out << endl;

    for(auto t: ts) t->codegen(this);
}

void BlockCodegen::define_prod(const std::string& name, const std::vector<Type*>& types) {
    _out << "class " << name << "_t;" << endl;
    for(auto t: types) t->define(this);
}
void BlockCodegen::declare_prod(const std::string& name, const std::vector<Type*>& types) {
    /* Type definition */
    _out << "class " << name << "_t {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    " << types[i]->ref(this) << " m" << i << ";" << endl;
    }
    _out << endl;

    _out << "public:" << endl;
    _out << "    static " << ref_prod(name, types) << " build(";
    _out << types[0]->ref(this) << " m0";
    for(size_t i = 1; i < types.size(); ++i) {
        _out << "," << types[i]->ref(this) << " m" << i;
    }
    _out << ");" << endl;
    _out << "    template <typename T, typename F>" << endl;
    _out << "    static T match(" << ref_prod(name,types) << " v, const F& f);" << endl;
    _out << "};" << endl;

    for(auto t : types) t->declare(this);
}
void BlockCodegen::gen_prod(const std::string& name, const std::vector<Type*>& types) {
    /* Dependencies */
    for(auto t : types) t->codegen(this);

    _out << ref_prod(name, types) << " " << name << "_t::build(";
    _out << types[0]->ref(this) << " m0";
    for(size_t i = 1; i < types.size(); ++i) {
        _out << "," << types[i]->ref(this) << " m" << i;
    }
    _out << ") {" << endl;
    /* Find one non native, and try to build next to it */
    size_t non_native = -1;
    for(size_t i = 0; i < types.size(); ++i) {
        if(!types[i]->is_native(_defined)) {
            non_native = i;
            break;
        }
    }
    _out << "    block0* blk = ";
    if(non_native == (size_t)-1) {
        _out << "first_block;" << endl;
    } else {
        _out << "(block0*)m" << non_native << ";" << endl;
    }

    _out << "    " << ref_prod(name, types) << " ptr = "
        << "(" << ref_prod(name, types) << ")"
        << "block_malloc<block" << _blocks.size() - 1 << ">"
        << "((char*)blk, sizeof(" << name << "_t));" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    ptr->m" << i << " = m" << i << ";" << endl;
    }
    _out << "    first_block = (block0*)ptr;" << endl;
    _out << "    return ptr;" << endl;
    _out << "}" << endl;
    _out << endl;

    _out << "template <typename T, typename F>" << endl;
    _out << "T " << name << "_t::match(" << ref_prod(name,types) << " v, const F& f) {" << endl;
    _out << "    return f(v->m0";
    for(size_t i = 1; i < types.size(); ++i) _out << ", v->m" << i;
    _out << ");" << endl;
    _out << "}" << endl;
    _out << endl;
}
std::string BlockCodegen::ref_prod(const std::string& name, const std::vector<Type*>& types) const {
    return name + "_t*";
}

void BlockCodegen::define_sum(const std::string& name, const std::vector<Type*>& types) {
    _out << "class " << name << "_t;" << endl;
    for(auto t: types) t->define(this);
}
void BlockCodegen::declare_sum(const std::string& name, const std::vector<Type*>& types) {
    _out << "class " << name << "_t {" << endl;
    _out << "    union {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "        " << types[i]->ref(this) << " m" << i << ";" << endl;
    }
    _out << "    };" << endl;
    _out << "    char type;" << endl;
    _out << endl;

    _out << "public:" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    static " << ref_sum(name, types) << " build_" << types[i]->base_name()
            << "(" << types[i]->ref(this) << " b);" << endl;
    }
    _out << "    template <typename T";
    for(size_t i = 0; i < types.size(); ++i) _out << ", typename F" << i;
    _out << ">" << endl;
    _out << "    static T match(" << ref_sum(name,types) << " v";
    for(size_t i = 0; i < types.size(); ++i) {
        _out << ", const F" << i << "& f" << i;
    }
    _out << ");" << endl;
    _out << "};" << endl;
    for(auto t : types) t->declare(this);
}
void BlockCodegen::gen_sum(const std::string& name, const std::vector<Type*>& types) {
    /* Dependencies */
    for(auto t : types) t->codegen(this);

    for(size_t i = 0; i < types.size(); ++i) {
        _out << ref_sum(name, types) << " " << name << "_t::build_" << types[i]->base_name()
            << "(" << types[i]->ref(this) << " b) {" << endl;
        int non_native = types[i]->is_native(_defined) ? 0 : 1;
        _out << "    block0* blk = "
            << (non_native ? "first_block" : "b")
            << ";" << endl;

        _out << "    " << ref_sum(name, types) << " ptr = "
            << "(" << ref_sum(name, types) << ")"
            << "block_malloc<block" << _blocks.size() - 1 << ">"
            << "((char*)blk, sizeof(" << name << "_t));" << endl;
        _out << "    ptr->type = " << i << ";" << endl;
        _out << "    ptr->m" << i << " = b;" << endl;
        _out << "    first_block = (block0*)ptr;" << endl;
        _out << "    return ptr;" << endl;
        _out << "}" << endl;
        _out << endl;
    }

    _out << "    template <typename T";
    for(size_t i = 0; i < types.size(); ++i) _out << ", typename F" << i;
    _out << ">" << endl;
    _out << "T " << name << "_t::match(" << ref_sum(name,types) << " v";
    for(size_t i = 0; i < types.size(); ++i) {
        _out << ", const F" << i << "& f" << i;
    }
    _out << ") {" << endl;
    _out << "    switch(v->type) {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "        case " << i << ": return f" << i
            << "(v->m" << i << "); break;" << endl;
    }
    _out << "    }" << endl;
    _out << "}" << endl;
    _out << endl;
}
std::string BlockCodegen::ref_sum(const std::string& name, const std::vector<Type*>& types) const {
    return name + "_t*";
}

void BlockCodegen::define_basic(const std::string& name) {
    /* Nothing to do */
}
void BlockCodegen::declare_basic(const std::string& name) {
    /* Nothing to do */
}
void BlockCodegen::gen_basic(const std::string& name) {
    /* Nothing to do */
}
std::string BlockCodegen::ref_basic(const std::string& name) const {
    auto it = _defined.find(name);
    if(it != _defined.end()) {
        return it->second->ref(this);
    } else return name;
}
