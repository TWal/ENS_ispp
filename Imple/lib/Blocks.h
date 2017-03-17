
#ifndef DEF_BLOCKS
#define DEF_BLOCKS

#include <iterator>
#include <map>

template < size_t SizeSub
         , size_t NbSubs
         , size_t FillingMax
         , size_t FillingMin
         >
struct block {
    /* Allocate a block next to the one in parameters
     * Returns NULL if FillingMax is reached
     */
    static char* allocate(char* block, char* next_to);
    /* Free a sub block.
     * Returns true if FillingMin is reached
     */
    static bool free(char* block, char* sub);
    /* Count the number of subblocks free.
     */
    static size_t filling(char* block);
    /* Take two blocks (assuming the second one is
     * empty, not initialized) and split the data in
     * the first between the two
     */
    static std::map<char*,char*> split(char* block, char* nw);
    /* Takes two blocks and merge the content of the second
     * in the first.
     */
    static std::map<char*,char*> merge(char* dest, char* block);
};

#endif//DEF_BLOCKS

