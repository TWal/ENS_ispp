
#include <BasicCodegen.h>
#include <map>
#include <iostream>
using namespace std;

void BasicCodegen::gen_sum(const std::string& name, const std::vector<Type*>& types) {
    /* Dependencies */
    for(auto d : types) d->codegen(this);

    /* Type definition */
    _out << "class " << name << "_t {" << endl;
    _out << "    size_t type;" << endl;
    _out << "    union {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "        " << types[i]->ref(this) << " m" << i << ";" << endl;
    }
    _out << "    };" << endl;
    _out << endl;

    _out << "public:" << endl;
    /* Building */
    for(size_t i = 0; i< types.size(); ++i) {
        _out << "    static " << ref_sum(name, types) << " build_" << types[i]->base_name()
            << "(" << types[i]->ref(this) << " b) {" << endl;
        _out << "        " << ref_sum(name,types) << " ret = new " << name << "_t;" << endl;
        _out << "        ret->type = " << i << ";" << endl;
        _out << "        ret->m" << i << " = b;" << endl;
        _out << "        return ret;" << endl;
        _out << "    }" << endl;
        _out << endl;
    }

    /* Matching */
    _out << "    template <typename T>" << endl;
    _out << "    static T match(" << ref_sum(name,types) << " v";
    for(size_t i = 0; i < types.size(); ++i) {
        _out << ", const function<T(" << types[i]->ref(this) << ")>& f" << i;
    }
    _out << ") {" << endl;
    _out << "        switch(v->type) {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "            case " << i << ": return f" << i
            << "(v->m" << i << ");" << endl;
    }
    _out << "        }" << endl;
    _out << "    }" << endl;
    _out << "};" << endl;
    _out << endl;
}

void BasicCodegen::declare_sum(const std::string& name, const std::vector<Type*>&) {
    _out << "class " << name << "_t;" << endl;
}

std::string BasicCodegen::ref_sum(const std::string& name, const std::vector<Type*>&) const {
    return name + "_t*";
}

void BasicCodegen::declare_prod(const std::string& name, const std::vector<Type*>&) {
    _out << "class " << name << "_t;" << endl;
}

void BasicCodegen::gen_prod(const std::string& name, const std::vector<Type*>& types) {
    /* Dependencies */
    for(auto d : types) d->codegen(this);

    /* Type definition */
    _out << "class " << name << "_t {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    " << types[i]->ref(this) << " m" << i << ";" << endl;
    }
    _out << endl;

    _out << "public:" << endl;
    /* Building */
    _out << "    static " << ref_prod(name,types) << " build(";
    _out << types[0]->ref(this) << " m0";
    for(size_t i = 1; i < types.size(); ++i) {
        _out << "," << types[i]->ref(this) << " m" << i;
    }
    _out << ") {" << endl;
    _out << "        " << ref_prod(name,types) << " bd = new " << name << "_t;" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "        bd->m" << i << " = m" << i << ";" << endl;
    }
    _out << "        return bd;" << endl;
    _out << "    }" << endl;
    _out << endl;

    /* Matching */
    _out << "    template <typename T>" << endl;
    _out << "    static T match(" << ref_prod(name,types) << " v, const std::function<T("
        << types[0]->ref(this);
    for(size_t i = 1; i < types.size(); ++i) {
        _out << ", " << types[i]->ref(this);
    }
    _out << ")>& f) {" << endl;
    _out << "        return f(";
    for(size_t i = 0; i < types.size(); ++i) {
        _out << (i == 0 ? "" : ", ") << "v->m" << i;
    }
    _out << ");" << endl;
    _out << "    }" << endl;

    _out << "};" << endl;
    _out << endl;
}

std::string BasicCodegen::ref_prod(const std::string& name, const std::vector<Type*>&) const {
    return name + "_t*";
}

void BasicCodegen::declare_basic(const std::string&) {
    /* Nothing to do */
}

void BasicCodegen::gen_basic(const std::string&) {
    /* Nothing to do */
}

std::string BasicCodegen::ref_basic(const std::string& name) const {
    auto it = _defined.find(name);
    if(it != _defined.end()) {
        return it->second->ref(this);
    } else return name;
}

