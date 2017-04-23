#ifndef COMPENTRY_H
#define COMPENTRY_H

#include <cstdint>
#include <string>
#include <vector>

struct CompEntry{
    std::string name;
    uint32_t head_size; // th_i
    uint32_t val; // v_i
    int nb6Pt; // s_i
    int size(){return ((head_size + 6 * nb6Pt + 7)/8)*8;} // tt_i
};

struct Entry {
    std::string name;
    int nbPt;
    double p;
    double cost(int l) const {
        return p*(((l + (1+6*nbPt) + 7)/8)*8);
    }
};

std::vector<CompEntry> optimizeEntries(const std::vector<Entry>& entries);

#endif
