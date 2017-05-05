
#ifndef BLOCKCODEGEN_H
#define BLOCKCODEGEN_H

#include <iostream>
#include "Type.h"

struct Block {
    int size;
    int fmax;
    int fmin;
};

class BlockCodegen : public Codegen{
    std::vector<Block> _blocks;
public:
    BlockCodegen(std::ostream& outHeader, std::ostream& outSource, const std::vector<Block>& blks)
        : Codegen(outHeader,outSource), _blocks(blks)
    {}

    void declare(const FullType& ft);
    void define(const FullType& ft);
    void implement(const FullType& ft);
};


#endif

