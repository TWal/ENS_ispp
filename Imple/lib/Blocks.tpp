
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>

#define DEBUG cerr << __LINE__ << endl

inline int header_offset(char* block, int off) {
    block += off / 8;
    return 1 - ((*block >> (off % 8)) & 0x01);
}
inline void header_unset(char* block, int off) {
    block += off / 8;
    *block |= 1 << (off % 8);
}
inline void header_set(char* block, int off) {
    block += off / 8;
    *block &= ~(1 << (off % 8));
}
    
template < typename T
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
block<T,NbSubs,FillingMax,FillingMin>::block() {
    /* Set the header to 1 for all empty blocks, and 0 for blocks outside of
     * range
     */
    size_t off1 = (NbSubs - 1) / 8;
    size_t rst  = (NbSubs - 1) % 8;
    size_t off2 = (NbSubs - 1) / 8 + (rst != 0);
    std::memset(this, 0xff, off1);
    *(char*)(this + off1) = 0xff - ((1 << rst) - 1);
    std::memset(this + off2, 0, SizeSub - off2);
}

template < typename T
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
char* block<T,NbSubs,FillingMax,FillingMin>::malloc(char* next_toT, size_t size) {
    assert((char*)this == next_toT - ((intptr_t)next_toT % Size));

    intptr_t rst = (intptr_t)next_toT % SizeSub;
    T* sub = (T*)(next_toT - rst);
    char* nw = sub->malloc(next_toT, size);
    if(!nw) {
        sub = allocate(sub);
        if(!sub) return NULL;
        char* ini = sub->init();
        nw = sub->malloc(ini, size);
        assert(nw);
    }
    return nw;
}

template < typename T
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
char* block<T,NbSubs,FillingMax,FillingMin>::init() {
    T* sub = allocate((T*)(data + Size));
    if(!sub) return NULL;
    char* ini = sub->init();
    assert(ini);
    return ini;
}

template < typename T
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
T* block<T,NbSubs,FillingMax,FillingMin>::allocate(T* next_toT) {
    char* next_to = (char*)next_toT;
    if(filling() >= FillingMax) return NULL;
    assert(next_to - data > 0);
    assert((size_t)(next_to - data) <= Size);

    size_t hd = SizeSub;
    size_t offset = std::distance(data + hd, next_to) / SizeSub;

    /* TODO use bsr assembly instruction */
    /* asm("mov %%eax, %0, ... %9" : "=r"(var) : "r"(var) : ) */
    int noff = offset - 1;
    while(noff >= 0 && header_offset(data,noff) == 1) --noff;
    if(noff < 0) return NULL;

    header_set(data, noff);
    char* addr = data + hd + noff * SizeSub;
    new(addr) T;

    assert((intptr_t)addr % SizeSub == 0);
    return (T*)addr;
}

/* Call T destructor */
template < typename T
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
bool block<T,NbSubs,FillingMax,FillingMin>::free(T* sub) {
    size_t hd = SizeSub;
    size_t offset = std::distance(data + hd, (char*)sub) / SizeSub;
    assert(header_offset(data,offset) == 1);
    header_unset(data,offset);
    return filling() <= FillingMin;
}

template < size_t Size >
charblock<Size>::charblock() {
    *(char*)this = 0;
}
template < size_t Size >
char* charblock<Size>::malloc(char*, size_t size) {
    assert(size < Size); /* If not, it will fail in an awful way */
    char* used = (char*)this;
    if(*used + size >= Size) return NULL;
    *used += size;
    return (char*)this + Size - *used;
}
    
template < size_t Size >
char* charblock<Size>::init() {
    return (char*)this;
}

template < typename T >
char* block_init() {
    const size_t SizeSub = sizeof(T);
    T* block;
    char* nw;

    block = (T*)aligned_alloc(SizeSub, SizeSub);
    assert(block);
    new(block) T;
    nw = block->init();
    assert(nw);
    return nw;
}

template < typename T >
char* block_malloc(char* next_to, size_t size) {
    const size_t SizeSub = sizeof(T);
    char* nw = NULL;
    T* block;

    if(next_to) {
        block = (T*)(next_to - ((intptr_t)next_to % SizeSub));
        nw = block->malloc(next_to, size); 
    }
    if(!nw) {
        /* Our use of aligned_alloc here is non-standart : despite being introduced
         * in C11, it is only planned for c++17. Thankfully, gcc support this
         * feature as of 5.4.0.
         */
        block = (T*)aligned_alloc(SizeSub, SizeSub);
        assert(block);
        new(block) T;
        char* ini = block->init();
        assert(ini);
        nw = block->malloc(ini, size);
        assert(nw);
    }
    return nw;
}

