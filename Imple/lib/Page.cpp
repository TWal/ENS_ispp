#include"Page.h"
#include <cstdlib>

int Page::nextId = 0;
std::vector<Page*> tmppages; // size should not be more than 2 or 3.

std::vector<Page*> compPages; // when page filling goes under minAlpha, it goes here
Page* current; // should not be in compPages.

void* Page::clAlloc(){
    assert(bitset);
    //if(!bitset) return nullptr;
    return &data[bsf(bitset)-1];
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
        void* vdest = aligned_alloc(0x1000,0x1000);
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
        // todo Think about mmap anonymous.
        void* vdest = aligned_alloc(0x1000,0x1000);
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

//int main(){}

// TODO free
