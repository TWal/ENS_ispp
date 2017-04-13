
#include "BlockCodegen.h"
#include <iostream>
#include <sstream>
using namespace std;

void BlockCodegen::define(const std::vector<Type*>& ts) {
    /* Include the Blocks.h header.
     * The block library is defined there, and all necessary include are
     * presetn there.
     */
    _out << "#include \"Blocks.h\"" << endl;

    /* Typedef all blocks level, creating the types block0, block1 ... */
    _out << "typedef charblock<" << _blocks[0].size << "> block0;" << endl;
    for(size_t i = 1; i < _blocks.size(); ++i) {
        _out << "typedef block<block" << i - 1 << ", "
            << _blocks[i].size << ", "
            << _blocks[i].filling_max << ", "
            << _blocks[i].filling_min
            << "> block" << i << ";" << endl;
    }

    /* The first block is the initial block next to which we will build
     * the types with no pointers.
     */
    _out << "extern block0* first_block;" << endl;

    for(auto t: ts) t->define(this);
}
void BlockCodegen::declare(const std::vector<Type*>& ts) {
    for(auto t: ts) t->declare(this);
}
void BlockCodegen::codegen(const std::vector<Type*>& ts) {
    _out << "block0* first_block;" << endl;

    /* Create a function initialising first_block by calling block_init
     * from the block library.
     */
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












/******************************************************************************
 ****************************** Product Types *********************************
 ******************************************************************************/

void BlockCodegen::define_prod(const std::string& name, const std::vector<Type*>& types) {
    _out << "class " << name << "_t;" << endl;
    for(auto t: types) t->define(this);
}







void BlockCodegen::declare_prod(const std::string& name, const std::vector<Type*>& types) {
    /* Type definition */
    _out << "class " << name << "_t {" << endl;
    /* A product is simply a struct with all its subtypes in it. */
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    " << types[i]->ref(this) << " m" << i << ";" << endl;
    }
    _out << endl;

    _out << "public:" << endl;
    /* The building function takes all the subtypes as arguments and create the
     * product type.
     */
    _out << "    static " << ref_prod(name, types) << " build(";
    _out << types[0]->ref(this) << " m0";
    for(size_t i = 1; i < types.size(); ++i) {
        _out << "," << types[i]->ref(this) << " m" << i;
    }
    _out << ");" << endl;

    /* Generate a getter per subtype. */
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    static " << types[i]->ref(this) << " get" << i << "("
            << ref_prod(name, types) << " v);" << endl;
    }
    _out << "};" << endl;

    for(auto t : types) t->declare(this);
}







void BlockCodegen::gen_prod(const std::string& name, const std::vector<Type*>& types) {
    /* Dependencies */
    for(auto t : types) t->codegen(this);



    /* Generating the build function for product types */
    _out << ref_prod(name, types) << " " << name << "_t::build(";
    /* Declaring all its arguments */
    _out << types[0]->ref(this) << " m0";
    for(size_t i = 1; i < types.size(); ++i) {
        _out << "," << types[i]->ref(this) << " m" << i;
    }
    _out << ") {" << endl;

    /* Loop over all its arguments, and try to find one that is non native
     * to build near. We assume there can't more than 255 subtypes, thus
     * 256 is an invalid value used to detect when every subtype is native.
     */
    size_t non_native = 256;
    for(size_t i = 0; i < types.size(); ++i) {
        if(!types[i]->is_native(_defined)) {
            non_native = i;
            break;
        }
    }

    /* Build next to the first_block if every subtype is native, else build
     * next to the first one that is non native.
     */
    _out << "    block0* blk = ";
    if(non_native == 256) {
        _out << "first_block;" << endl;
    } else {
        _out << "(block0*)m" << non_native << ";" << endl;
    }

    /* Call block_malloc to allocate a place for our new type. */
    _out << "    " << ref_prod(name, types) << " ptr = "
        << "(" << ref_prod(name, types) << ")"
        << "block_malloc<block" << _blocks.size() - 1 << ">"
        << "((char*)blk, sizeof(" << name << "_t));" << endl;

    /* Fill the type with the right values. */
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    ptr->m" << i << " = m" << i << ";" << endl;
    }

    /* Reset the first block to the newly used one: it prevents first_block
     * to fill up, necessitating a new allocation for every fully native type
     * allocation
     */
    _out << "    first_block = (block0*)ptr;" << endl;
    _out << "    return ptr;" << endl;
    _out << "}" << endl;
    _out << endl;




    /* As of now, a getter simply returns the value. */
    for(size_t i = 0; i < types.size(); ++i) {
        _out << types[i]->ref(this) << " " << name << "_t::get" << i << "("
            << ref_prod(name, types) << " v) {" << endl;
        _out << "    return v->m" << i << ";" << endl;
        _out << "}" << endl;
    }
}







std::string BlockCodegen::ref_prod(const std::string& name, const std::vector<Type*>& types) const {
    return name + "_t*";
}











/******************************************************************************
 **************************** Sum Types ***************************************
 ******************************************************************************/

void BlockCodegen::define_sum(const std::string& name, const std::vector<Type*>& types) {
    _out << "class " << name << "_t;" << endl;
    for(auto t: types) t->define(this);
}







void BlockCodegen::declare_sum(const std::string& name, const std::vector<Type*>& types) {
    _out << "class " << name << "_t {" << endl;
    /* As sum type is the union of its subtypes, plus a type field identifying
     * which one it is. Because we're using a char for the last field, the
     * number of subtypes is limited to 255
     */
    _out << "    union {" << endl;
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "        " << types[i]->ref(this) << " m" << i << ";" << endl;
    }
    _out << "    };" << endl;
    _out << "    char type;" << endl;
    _out << endl;

    _out << "public:" << endl;
    /* There is a building function for every subtype. */
    for(size_t i = 0; i < types.size(); ++i) {
        _out << "    static " << ref_sum(name, types) << " build_" << types[i]->base_name()
            << "(" << types[i]->ref(this) << " b);" << endl;
    }

    /* The match function take as argument as many function as there are
     * subtypes.
     */
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



    /* Create all the building functions */
    for(size_t i = 0; i < types.size(); ++i) {
        _out << ref_sum(name, types) << " " << name << "_t::build_" << types[i]->base_name()
            << "(" << types[i]->ref(this) << " b) {" << endl;
        /* Check if the type we're building from is native or not */
        int non_native = types[i]->is_native(_defined) ? 0 : 1;

        /* If it is, build next to first_block, else next to it */
        _out << "    block0* blk = "
            << (non_native ? "first_block" : "b")
            << ";" << endl;

        /* Call block_malloc to allocate space for the new type */
        _out << "    " << ref_sum(name, types) << " ptr = "
            << "(" << ref_sum(name, types) << ")"
            << "block_malloc<block" << _blocks.size() - 1 << ">"
            << "((char*)blk, sizeof(" << name << "_t));" << endl;

        /* Fill the newly created type */
        _out << "    ptr->type = " << i << ";" << endl;
        _out << "    ptr->m" << i << " = b;" << endl;

        /* As for the product types, reset the first_block pointer */
        _out << "    first_block = (block0*)ptr;" << endl;
        _out << "    return ptr;" << endl;
        _out << "}" << endl;
        _out << endl;
    }



    /* The match function */
    /* Template on as many function as they are subtypes ... */
    _out << "    template <typename T";
    for(size_t i = 0; i < types.size(); ++i) _out << ", typename F" << i;
    _out << ">" << endl;
    /* ... and add them as arguments */
    _out << "T " << name << "_t::match(" << ref_sum(name,types) << " v";
    for(size_t i = 0; i < types.size(); ++i) {
        _out << ", const F" << i << "& f" << i;
    }
    _out << ") {" << endl;

    /* Switch on the type field to determine which type it really is, and apply
     * the function to it.
     */
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










/******************************************************************************
 *************************** Basic Types **************************************
 ******************************************************************************/

/* A basic type is either an already defined type referenced, or a native type.
 * The difference is made with the _defined map, which contain all the already
 * defined types.
 */
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
    /* If the type is non native, use its ref method */
    auto it = _defined.find(name);
    if(it != _defined.end()) {
        return it->second->ref(this);
    } else return name;
}

