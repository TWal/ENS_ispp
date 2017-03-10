
#include <BasicCodegen.h>
#include <map>
#include <iostream>
using namespace std;
    
void BasicCodegen::define(const std::vector<Type*>& ts) {
    _out << "extern unsigned long long __count;" << endl;
    for(auto t: ts) t->define(this);
}

void BasicCodegen::declare(const std::vector<Type*>& ts) {
    for(auto t: ts) t->declare(this);
}

void BasicCodegen::codegen(const std::vector<Type*>& ts) {
    _out << "unsigned long long __count = 0;" << endl;
    for(auto t: ts) t->codegen(this);
}
    
void BasicCodegen::define_prod(const std::string& name, const std::vector<Type*>& types) {
    _out << "class " << name << "_t;" << endl;
    for(auto t : types) t->define(this);
}

void BasicCodegen::gen_sum(const std::string& name, const std::vector<Type*>& types) {
    /* Dependencies */
    for(auto d : types) d->codegen(this);

    /* Building */
    for(size_t i = 0; i < types.size(); ++i) {
        _out << ref_sum(name, types) << " " << name << "_t::build_" << types[i]->base_name()
            << "(" << types[i]->ref(this) << " b) {" << endl;
        _out << "    " << ref_sum(name,types) << " ret = new " << name << "_t;" << endl;
        _out << "    ret->type = " << i << ";" << endl;
        _out << "    ret->m" << i << " = b;" << endl;

        /* Logging */
        _out << "    ret->id = ++__count;" << endl;
        _out << "    cerr << ret->id << \" < " << name << " as " << types[i]->base_name() << " \" << "
             << (types[i]->is_native(_defined) ? "\"" + types[i]->base_name() + "=\" << b"
                                               : "b->get_id()")
             << " << endl;" << endl;
        /* End of logging */

        _out << "    return ret;" << endl;
        _out << "}" << endl;
        _out << endl;
    }

    /* Matching */
    _out << "template <typename T>" << endl;
    _out << "T " << name << "_t::match(" << ref_sum(name,types) << " v";
    for(size_t i = 0; i < types.size(); ++i) {
        _out << ", const function<T(" << types[i]->ref(this) << ")>& f" << i;
    }
    _out << ") {" << endl;
    _out << "    switch(v->type) {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "        case " << i << ": " << endl;
        /* Logging */
        _out << "            cerr << \"" << name << " as " << types[i]->base_name() << " \" << ";
        if(types[i]->is_native(_defined)) _out << types[i]->base_name() << " << \"=\" << v->m" << i;
        else _out << "v->m" << i << "->get_id()";
        _out << " << \" < \" << v->id << endl;" << endl;
        /* End of logging */
        _out << "            return f" << i
            << "(v->m" << i << ");" << endl;
    }
    _out << "    }" << endl;
    _out << "}" << endl;

    /* Getting id */
    _out << endl;
    _out << "unsigned long long " << name << "_t::get_id() const {" << endl;
    _out << "   return id;" << endl;
    _out << "}" << endl;
    _out << endl;
}

void BasicCodegen::declare_sum(const std::string& name, const std::vector<Type*>& types) {
    /* Type definition */
    _out << "class " << name << "_t {" << endl;
    _out << "    unsigned long long id;" << endl;
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
    _out << "    template <typename T>" << endl;
    _out << "    static T match(" << ref_sum(name,types) << " v";
    for(size_t i = 0; i < types.size(); ++i) {
        _out << ", const function<T(" << types[i]->ref(this) << ")>& f" << i;
    }
    _out << ");" << endl;
    _out << "    unsigned long long get_id() const;" << endl;
    _out << "};" << endl;
    for(auto t : types) t->declare(this);
}

std::string BasicCodegen::ref_sum(const std::string& name, const std::vector<Type*>&) const {
    return name + "_t*";
}
    
void BasicCodegen::define_sum(const std::string& name, const std::vector<Type*>& types) {
    _out << "class " << name << "_t;" << endl;
    for(auto t : types) t->define(this);
}

void BasicCodegen::declare_prod(const std::string& name, const std::vector<Type*>& types) {
    /* Type definition */
    _out << "class " << name << "_t {" << endl;
    _out << "    unsigned long long id;" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    " << types[i]->ref(this) << " m" << i << ";" << endl;
    }
    _out << endl;

    _out << "public:" << endl;
    _out << "    static " << ref_prod(name,types) << " build(";
    _out << types[0]->ref(this) << " m0";
    for(size_t i = 1; i < types.size(); ++i) {
        _out << "," << types[i]->ref(this) << " m" << i;
    }
    _out << ");" << endl;
    _out << "template <typename T>" << endl;
    _out << "static T match(" << ref_prod(name,types) << " v, const std::function<T("
        << types[0]->ref(this);
    for(size_t i = 1; i < types.size(); ++i) {
        _out << ", " << types[i]->ref(this);
    }
    _out << ")>& f);" << endl;
    _out << "    unsigned long long get_id() const;" << endl;
    _out << "};" << endl;
    for(auto t : types) t->declare(this);
}

void BasicCodegen::gen_prod(const std::string& name, const std::vector<Type*>& types) {
    /* Dependencies */
    for(auto d : types) d->codegen(this);

    /* Building */
    _out << ref_prod(name,types) << " " << name << "_t::build(";
    _out << types[0]->ref(this) << " m0";
    for(size_t i = 1; i < types.size(); ++i) {
        _out << "," << types[i]->ref(this) << " m" << i;
    }
    _out << ") {" << endl;
    _out << "    " << ref_prod(name,types) << " bd = new " << name << "_t;" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    bd->m" << i << " = m" << i << ";" << endl;
    }

    /* Logging */
    _out << "    bd->id = ++__count;" << endl;
    _out << "    cerr << bd->id << \" < " << name << " (\" << ";
    _out << (types[0]->is_native(_defined) ? "\"" + types[0]->base_name() + "=\" << m0"
                                           : "m0->get_id()");
    for(size_t i = 1; i < types.size(); ++i) {
        if(types[i]->is_native(_defined)) _out << " << \", " << types[i]->base_name() << "=\" << m" << i;
        else _out << " << \", \" << m" << i << "->get_id()";
    }
    _out << " << \")\" << endl;" << endl;
    /* End of logging */

    _out << "    return bd;" << endl;
    _out << "}" << endl;
    _out << endl;

    /* Matching */
    _out << "template <typename T>" << endl;
    _out << "T " << name << "_t::match(" << ref_prod(name,types) << " v, const std::function<T("
        << types[0]->ref(this);
    for(size_t i = 1; i < types.size(); ++i) {
        _out << ", " << types[i]->ref(this);
    }
    _out << ")>& f) {" << endl;
    /* Logging */
    _out << "    cerr << \"" << name << " (\" << ";
    if(!types[0]->is_native(_defined)) _out << "v->m0->get_id()";
    else _out << "\"" << types[0]->base_name() << "=\" << v->m0";
    for(size_t i = 1; i < types.size();++i) {
        _out << " << \", \" << ";
        if(!types[i]->is_native(_defined)) _out << "v->m" << i << "->get_id()";
        else _out << "\"" << types[i]->base_name() << "=\" << v->m" << i;
    }
    _out << " << \") < \" << v->get_id() << endl;" << endl;
    /* End of logging */
    _out << "    return f(";
    for(size_t i = 0; i < types.size(); ++i) {
        _out << (i == 0 ? "" : ", ") << "v->m" << i;
    }
    _out << ");" << endl;
    _out << "}" << endl;

    /* Getting id */
    _out << endl;
    _out << "unsigned long long " << name << "_t::get_id() const {" << endl;
    _out << "   return id;" << endl;
    _out << "}" << endl;
    _out << endl;
}

std::string BasicCodegen::ref_prod(const std::string& name, const std::vector<Type*>&) const {
    return name + "_t*";
}
    
void BasicCodegen::define_basic(const std::string&) {
    /* Nothing to do */
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

