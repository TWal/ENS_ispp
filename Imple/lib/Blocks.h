
#ifndef DEF_BLOCKS
#define DEF_BLOCKS

#include <iterator>
#include <cstring>
#include <cstdlib>
#include <iostream>

/* Assumes T has :
 * - a constructor without arguments
 * - a function create
 * - a function init
 * Furthermore, assumes sizeof(T) >= 8 and NbSubs < 64
 */
template < typename T
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
struct block {
    const static size_t SizeSub = sizeof(T);
    const static size_t Size    = NbSubs * SizeSub;
    char data[Size];

    block();

    /* Recusively (on type) allocate a new block of given size
     * the closest possible to the one given in parameter.
     * Returns NULL if it wasn't possible.
     */
    char* malloc(char* next_to, size_t size);
    /* Recursively allocate a new level 0 block at the end
     * of the block. Assume the block is empty (ie just created)
     */
    char* init();
    /* Allocate a block next to the one in parameters
     * Returns NULL if FillingMax is reached
     * If block is aligned to NbSubs * SizeSub, the block allocated
     * will be alligned to SizeSub.
     */
    T* allocate(T* next_toT);
    /* Free a sub block.
     * Returns true if FillingMin is reached
     */
    bool free(T* sub);
    /* Count the number of subblocks free.
     */
    inline size_t filling() const {
        /* Here endianness doesn't matter : we only count the bits set to 1 */
        uint64_t bitmap = *(uint64_t*)data; 
        uint64_t cnt;
        asm("popcnt %1,%0;" : "=r" (bitmap) : "r" (cnt));
        return NbSubs - 1 - cnt;
    }
};

/* The basic block type. */
template < size_t Size >
struct charblock {
    char data[Size];
    charblock();
    char* malloc(char*, size_t size);
    char* init();
};

/* The global malloc */
template < typename T >
char* block_malloc(char* next_to, size_t size);

#include "Blocks.tpp"

#endif//DEF_BLOCKS

