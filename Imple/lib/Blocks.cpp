
#include "Blocks.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
using namespace std;

#define DEBUG cerr << __LINE__ << endl

#define HEADER_OFFSET(hd,off) (((*((hd) + ((off) / 8))) >> ((off) % 8)) & 0x01)
#define HEADER_SET(hd,off) ((*((hd) + ((off) / 8))) |= (1 << ((off) % 8)))
#define HEADER_UNSET(hd,off) ((*((hd) + ((off) / 8))) &= ~(1 << ((off) % 8)))

template < size_t SizeSub
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
char* block<SizeSub,NbSubs,FillingMax,FillingMin>::allocate(char* block, char* next_to) {
    if(filling(block) >= FillingMax) return NULL;
    assert(next_to - block > 0);
    assert((size_t)(next_to - block) <= NbSubs * SizeSub);
    // for(size_t i = 0; i < NbSubs; ++i) cout << HEADER_OFFSET(block, i);
    // cout << " -> ";

    size_t hd = SizeSub;
    size_t offset = distance(block + hd, next_to) / SizeSub;

    size_t noff = offset - 1;
    while(noff < numeric_limits<size_t>::max() && HEADER_OFFSET(block,noff) == 1) --noff;
    if(noff == numeric_limits<size_t>::max()) return NULL;

    HEADER_SET(block, noff);
    char* addr = block + hd + noff * SizeSub;
    memset(addr, 0, NbSubs);
    // for(size_t i = 0; i < NbSubs; ++i) cout << HEADER_OFFSET(block, i);
    // cout << endl;
    return addr;
}

template < size_t SizeSub
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
bool block<SizeSub,NbSubs,FillingMax,FillingMin>::free(char* block, char* sub) {
    size_t hd = SizeSub;
    size_t offset = distance(block + hd, sub) / SizeSub;
    assert(HEADER_OFFSET(block,offset) == 1);
    HEADER_UNSET(block,offset);
    return filling(block) <= FillingMin;
}

template < size_t SizeSub
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
size_t block<SizeSub,NbSubs,FillingMax,FillingMin>::filling(char* block) {
    size_t count = 0;
    /* TODO optimize */
    for(size_t off = 0; off < NbSubs; ++off) count += HEADER_OFFSET(block,off);
    return count;
}

template < size_t SizeSub
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
map<char*,char*> block<SizeSub,NbSubs,FillingMax,FillingMin>::split(char* block, char* nw) {
    /* TODO */
}

template < size_t SizeSub
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
map<char*,char*> block<SizeSub,NbSubs,FillingMax,FillingMin>::merge(char* dest, char* block) {
    /* TODO */
}



