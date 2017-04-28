
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>

#define DEBUG cerr << __LINE__ << endl

inline int header_offset(uint64_t* block, int off) {
    return 1 - ((*block >> off) & 1ll);
}
inline void header_unset(uint64_t* block, int off) {
    *block |= 1ll << off;
}
inline void header_set(uint64_t* block, int off) {
    *block &= ~(1ll << off);
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
    uint64_t *bitmap = (uint64_t*)data;
    *bitmap = (uint64_t)(1ll << (NbSubs - 1)) - 1;
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
        new(sub) T;
        char* ini = sub->init();
        assert(ini);
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
    new(sub) T;
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
    assert(next_to - data >= SizeSub);
    assert((size_t)(next_to - data) <= Size);

    size_t hd = SizeSub;
    size_t offset = std::distance(data + hd, next_to) / SizeSub;
    if(offset == 0) return NULL;

    uint64_t noff = offset;
    uint64_t bitmap = (*(uint64_t*)data) & ((1ll << noff) - 1ll);
    if(bitmap == 0) return NULL;
    /* TODO fill from the end if necessary */
    asm("bsr %1,%0;" : "=r" (noff) : "r" (bitmap));

    header_set((uint64_t*)data, noff);
    char* addr = data + hd + noff * SizeSub;
    new(addr) T;

    assert((intptr_t)addr % SizeSub == 0);
    assert((intptr_t)addr % Size != 0);
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
    assert(header_offset((uint64_t*)data,offset) == 1);
    header_unset((uint64_t*)data,offset);
    return filling() <= FillingMin;
}

template < typename T
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
size_t block<T,NbSubs,FillingMax,FillingMin>::level_0() const {
    size_t lvl0 = 0;
    for(size_t off = 0; off < NbSubs - 1; ++off) {
        if(header_offset((uint64_t*)data, off)) {
            T* sub = (T*)(data + (off + 1) * SizeSub);
            lvl0 += sub->level_0();
        }
    }
    return lvl0;
}

template < size_t Size >
charblock<Size>::charblock() {
    data[0] = 0;
}
template < size_t Size >
char* charblock<Size>::malloc(char*, size_t size) {
    assert(size < Size); /* If not, it will fail in an awful way */
    if(data[0] + size >= Size) return NULL;
    data[0] += size;
    return data + Size - data[0];
}

template < size_t Size >
char* charblock<Size>::init() {
    return data;
}

template < size_t Size >
size_t charblock<Size>::level_0() const {
    return data[0];
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
    T* block = NULL;

    if(next_to) {
        block = (T*)(next_to - ((intptr_t)next_to % SizeSub));
        nw = block->malloc(next_to, size); 
    }
    if(!nw) {
        /* Our use of aligned_alloc here is non-standard : despite being introduced
         * in C11, it is only planned for c++17. Thankfully, gcc support this
         * feature as of 5.4.0.
         */
        block = (T*)aligned_alloc(SizeSub, SizeSub);
        assert(block);
        assert((intptr_t)block % SizeSub == 0);
        new(block) T;
        char* ini = block->init();
        assert(ini);
        nw = block->malloc(ini, size);
        assert(nw);
    }
    return nw;
}

