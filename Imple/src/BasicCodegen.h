
#ifndef BASICCODEGEN_H
#define BASICCODEGEN_H

#include <iostream>
#include "Type.h"

class BasicCodegen : public Codegen{
public:
    BasicCodegen(std::ostream& outHeader,std::ostream& outSource) : Codegen(outHeader,outSource){}
    void declare(const FullType& ft);
    void define(const FullType& ft);
    void implement(const FullType& ft);
};


#endif

