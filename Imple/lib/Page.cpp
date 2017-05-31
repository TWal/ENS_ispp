#include"Page.h"
#include <cstdlib>
#include <sys/mman.h>

int Page::nextId = 0;
std::vector<Page*> tmppages; // size should not be more than 2 or 3.

std::vector<Page*> compPages; // when page filling goes under minAlpha, it goes here
Page* current; // should not be in compPages.

void* Page::clAlloc(){
    assert(bitset);
    //if(!bitset) return nullptr;
    return &data[bsf(bitset)-1];
}

inline void* pageAlloc(){
    return mmap(NULL, 0x1000, PROT_READ | PROT_WRITE,
            MAP_ANONYMOUS, -1, 0);
}

void* clTmpAlloc(){
    Page* dest = nullptr;
    for(auto p : tmppages){
        if(!p->full()){
            dest = p;
            break;
        }
    }
    if(!dest){
        void* vdest = pageAlloc();
        dest = new(vdest) Page(true);
        tmppages.push_back(dest);
    }
    return dest->clAlloc();
}

void setCurrent(){
    assert(!current);
    size_t maxFree = 0;
    Page* tmp = nullptr;
    for(auto p : compPages){
        if(p->nbFree() > maxFree){
            tmp = p;
            maxFree = p->nbFree();
        }
    }
    if(!tmp){
        void* vdest = pageAlloc();
        current = new(vdest) Page(false);
    }
    else current = tmp;
}

void* clAlloc(){
    assert(current);
    void* cl = current->clAlloc();
    assert(cl);
    if(current->full()){
        current = nullptr;
        setCurrent();
    }
    return cl;
}

struct _int24 {
    char bytes[3];
} __attribute ((__packed__));

char Page::getSize(int id) {
    struct _int24* msizes = (struct _int24*)sizes;
    uint32_t value = (uint32_t)msizes[id / 3].bytes[0]
        + (uint32_t)msizes[id / 3].bytes[1] << 8
        + (uint32_t)msizes[id / 3].bytes[2] << 16;
    return (char)(value >> ((id % 3) * 6)) & 0x3F;
}

void Page::setSize(int id, char s) {
    struct _int24* msizes = (struct _int24*)sizes;
    uint32_t value = (s & 0x3F) << ((id % 3) * 6);
    for(int i = 0; i < 3; ++i) {
        msizes[id / 3].bytes[i] |= value % 256;
        value >> 8;
    }
}

char Page::addToSize(int id, char s) {
    char ns = getSize(id);
    ns += s;
    setSize(id, ns);
    return ns;
}

void clfree(void* cl) {
    Page* pg = (Page*)((char*)cl - ((intptr_t)cl % 0x1000));
    int mid = ((intptr_t)cl % 0x1000) / 64;
    pg->setSize(mid, 0);
    pg->bitset |= 1 << mid;
    if(pg->id < 0) return;

    if(pg->nbFree() == 0 && compPages.size() >= Page::maxToFill) {
        pg->~Page();
        munmap(pg, 0x1000);
    } else if(pg->nbFree() == Page::minAlpha) {
        compPages.push_back(pg);
    }
}

void clfree(void* cl,int size) {
    Page* pg = (Page*)((char*)cl - ((intptr_t)cl % 0x1000));
    int id = ((intptr_t)cl % 0x1000) / 64;
    int sz = pg->getSize(id);
    assert(sz >= size);
    sz -= size;
    pg->setSize(id, sz);
    if(sz == 0) clfree(cl);
}

// int main(){}

// TODO free
