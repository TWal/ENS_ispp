#ifndef TYPE_H
#define TYPE_H

#include <cstddef>
#include <map>
#include <vector>
#include <string>
#include <iostream>

struct LeafType;
struct FullType{
    std::string name;
    std::map<std::string,std::vector<LeafType>> content;
};


struct LeafType{
    std::string name;
    FullType* pointedType;
    explicit LeafType(std::string nname): pointedType(nullptr)
        {name = std::move(nname);}
};

class Codegen{
    protected:
        std::ostream& _out;
        std::ostream& _outs;
    public:
        Codegen(std::ostream& outHeader,std::ostream& outSource)
            : _out(outHeader),_outs(outSource) {}
        void protoBuilder(std::ostream& out, std::string name,
                          const std::pair<std::string,std::vector<LeafType>>& constructor);
        void protoMatch(std::ostream& out, const FullType& ft);
        void protoFree(std::ostream& out, const FullType& ft);
        void headBuilder(std::ostream& out, std::string name,
                         const std::pair<std::string,std::vector<LeafType>>& constructor);
        void headMatch(std::ostream& out, const FullType& ft);
        void headFree(std::ostream& out, const FullType& ft);
        virtual void declare(const FullType& ft) = 0;
        virtual void define(const FullType& ft) = 0;
        virtual void implement(const FullType& ft) = 0;
        virtual void header() = 0; // set up header
};

void fillLink(std::vector<FullType>& ft);

std::ostream& operator << (std::ostream& out,const FullType& type);

#endif

