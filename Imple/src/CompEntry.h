#ifndef COMPENTRY_H
#define COMPENTRY_H

#include<cstdint>
#include<string>

struct CompEntry{
    std::string name;
    char head_size; // th_i
    char val; // v_i
    char nb6Pt; // s_i
    char size(){return (head_size + 6 * nb6Pt + 7)/8 *8;} // tt_i
};


#endif
