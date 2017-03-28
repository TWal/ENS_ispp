
#include "BlockCodegen.h"
#include <iostream>
using namespace std;

size_t BlockCodegen::real_size(size_t id) {
    if(id == 0) return _blocks[0].size;
    else        return _blocks[id].size * real_size(id - 1);
}
void BlockCodegen::find_block(const std::string& name) {
    _out << "char used = (char*)blks[0];" << endl;
    _out << "if(used + sizeof(" << name << "_t) > " << _blocks[0].size - 1 << ") {" << endl;
    for(size_t i = 1; i < _blocks.size(); ++i) {
        _out << string(' ', 4*(i+1)) << "__block blk" << i << " = block<"
            << real_size(i-1) << "," << _blocks[i].size << ","
            << _blocks[i].filling_max << "," << _blocks[i].filling_min
            << ">::allocate(blks[" << i << "], blks[" << i-1 << "]);" << endl;
        _out << string(' ', 4*(i+1)) << "if(!blk" << i << ") {" << endl;
    }
    _out << string(' ', 4*(_blocks.size() + 1)) << "/* Assumes malloc does not fails */" << endl;
    _out << string(' ', 4*(_blocks.size() + 1)) << "blks[" << _blocks.size() - 1
        << " = malloc(" << real_size(_blocks.size() - 1) << ");" << endl;
    for(size_t i = _blocks.size() - 1; i > 0; --i) {
        _out << string(' ', 4*(i+2)) << "blk" << i << " = block<"
            << real_size(i-1) << "," << _blocks[i].size << ","
            << _blocks[i].filling_max << "," << _blocks[i].filling_min
            << ">::allocate(blks[" << i << "], blks[" << i-1 << "]);" << endl;
        _out << "/* Cannot fails because the upper block is new, thus empty */" << endl;
        _out << string(' ', 4*(i+1)) << "}" << endl;
        _out << "blks[" << i - 1 << "] = blk" << i << ";" << endl;
    }
    _out << "    }" << endl;
    _out << "    memset(blks[0], 0, " << _blocks[0].size << ");" << endl;
    _out << "    *(char*)blks[0] = sizeof(" << name << "_t);" << endl;
    _out << "    " << name << "_t* ptr = (" << name << "_t*)(blks[0] + " << _blocks[0].size
        << " - sizeof(" << name << "_t));" << endl;
}

void BlockCodegen::define(const std::vector<Type*>& ts) {
    _out << "typedef void* __block;" << endl;
    _out << "extern array<__block," << _blocks.size() << "> first_block;" << endl;
    _out << "extern __block first_pos;" << endl;
    for(auto t: ts) t->define(this);
}
void BlockCodegen::declare(const std::vector<Type*>& ts) {
    for(auto t: ts) t->declare(this);
}
void BlockCodegen::codegen(const std::vector<Type*>& ts) {
    _out << "__block first_block;" << endl;
    _out << "__block first_pos;" << endl;
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
    _out << "    array<__block," << _blocks.size() << "> blks;" << endl;
    _out << "    static " << ref_prod(name, types) << " build(";
    _out << types[0]->ref(this) << " m0";
    for(size_t i = 1; i < types.size(); ++i) {
        _out << "," << types[i]->ref(this) << " m" << i;
    }
    _out << ");" << endl;
    _out << "template <typename T, typename F>" << endl;
    _out << "static T match(" << ref_prod(name,types) << " v, const F& f);" << endl;
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
    _out << "    array<__block" << _blocks.size() << ">& blks = ";
    if(non_native == (size_t)-1) _out << "first_block"; else _out << "m" << non_native << "->blks";
    _out << ";" << endl;
    _out << "    __block pos = ";
    if(non_native == (size_t)-1) _out << "first_pos"; else _out << "m" << non_native;
    _out << ";" << endl;

    find_block(name);
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    ptr->m" << i << " = m" << i << ";" << endl;
    }
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

    _out << "__block " << name << "_t::get_block() {" << endl;
    _out << "    return mblk;" << endl;
    _out << "}" << endl;
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
    _out << "    size_t type;" << endl;
    _out << "    union {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "        " << types[i]->ref(this) << " m" << i << ";" << endl;
    }
    _out << "    };" << endl;
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
        _out << ref_sum(name, types) << "build_" << types[i]->base_name()
            << "(" << types[i]->ref(this) << " b) {" << endl;
        int non_native = types[i]->is_native(_defined) ? 0 : 1;
        _out << "    array<__block" << _blocks.size() << ">& blks = ";
        if(!non_native) _out << "first_block"; else _out << "b->blks";
        _out << ";" << endl;
        _out << "    __block pos = ";
        if(!non_native) _out << "first_pos"; else _out << "b";
        _out << ";" << endl;
        find_block(name);

        _out << "    ptr->type = " << i << ";" << endl;
        _out << "    ptr->m" << i << ";" << endl;
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
