
#include "BlockCodegen.h"

void BlockCodegen::define(const std::vector<Type*>& ts) {
}
void BlockCodegen::declare(const std::vector<Type*>& ts) {
}
void BlockCodegen::codegen(const std::vector<Type*>& ts) {
}

void BlockCodegen::define_prod(const std::string& name, const std::vector<Type*>& types) {
}
void BlockCodegen::declare_prod(const std::string& name, const std::vector<Type*>& types) {
}
void BlockCodegen::gen_prod(const std::string& name, const std::vector<Type*>& types) {
}
std::string BlockCodegen::ref_prod(const std::string& name, const std::vector<Type*>& types) const {
}

void BlockCodegen::define_sum(const std::string& name, const std::vector<Type*>& types) {
}
void BlockCodegen::declare_sum(const std::string& name, const std::vector<Type*>& types) {
}
void BlockCodegen::gen_sum(const std::string& name, const std::vector<Type*>& types) {
}
std::string BlockCodegen::ref_sum(const std::string& name, const std::vector<Type*>& types) const {
}

void BlockCodegen::define_basic(const std::string& name) {
}
void BlockCodegen::declare_basic(const std::string& name) {
}
void BlockCodegen::gen_basic(const std::string& name) {
}
std::string BlockCodegen::ref_basic(const std::string& name) const {
}
