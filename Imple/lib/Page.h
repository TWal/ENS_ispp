#ifndef PAGE_H
#define PAGE_H

#include<cstdint>
#include<cstring>
#include<vector>
#include<deque>
#include<set>
#include<list>
#include <cassert>

using u64 = uint64_t;
using cacheLine = char[64];

inline size_t bsf(u64 val){
    size_t pos;
    asm("bsf %1,%0;" : "=r" (pos) : "r" (val));
    return pos;
}

inline size_t cnt(u64 val){
    size_t pos;
    asm("popcnt %1,%0;" : "=r" (pos) : "r" (val));
    return pos;
}


class Page{
    public:
        int magic; //0x7743de16
        int id;
        static int nextId;
        u64 bitset;
        char sizes[48]; // bit i is one in bitset iff sizes[i] == 0
        cacheLine data[63];
        char addToSize(int id, char s);
        char getSize(int id);
        void setSize(int id, char s);

        Page(bool tmp){
            assert((u64(this) & 0xFFF) == 0);
            // Useless : mmap gives a zero'd out page
            // memset(this,0,64);
            id = tmp ? -nextId++ : nextId++;
            bitset = 0x7FFFFFFFFFFFFFFFull;
#ifndef NDEBUG // for checking that a page is indeed a Page.
            magic = 0x7743de16;
#endif
        }
        // number of empty cachelines under which Page is going to refilling
        static const int minAlpha = 40;
        static const int maxToFill = 30;
        void* clAlloc();
        bool full() const {return !bitset;}
        size_t nbFree() const {return cnt(bitset);}
};

static_assert(sizeof(Page) == 0x1000);


extern std::vector<Page*> tmppages; // size should not be more than 2 or 3.

extern std::vector<Page*> compPages; // when page filling goes under minAlpha, it goes here
extern Page* current; // should not be in compPages.

void* clTmpAlloc();
void setCurrent();
void* clAlloc();
void  clfree(void* cl);
void  clfree(void* cl,int size);




#endif
