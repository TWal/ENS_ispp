#ifndef LOGELEM_H
#define LOGELEM_H

#include<vector>
#include <string>
#include <iostream>

namespace log{

    struct TypeEntry{
        TypeEntry (std::string n) : name(n){}
        std::string name;
        virtual void print(std::ostream& out) = 0;
    };

    struct ProdEntry : public TypeEntry{
        ProdEntry(std::string n,std::vector<int>mem) : TypeEntry(n),members(mem){}
        std::vector<int> members; // basic type with given size if negative
        inline void print(std::ostream& out){
            out << "(";
            for(auto v : members){
                out << v << ", ";
            }
            out << ")";
        }
    };

    struct SumEntry : public TypeEntry{
        SumEntry(std::string n,std::string c,int mem) : TypeEntry(n),Constructor(c),member(mem){}
        std::string Constructor;
        int member;
        inline void print(std::ostream& out){
            out << Constructor << " " << member;
        }
    };


    struct LogElem {
        bool construction;
        int entry;
        TypeEntry * Entry;
    };

    inline std::ostream& operator << (std::ostream& out, LogElem le){
        if (le.construction){
            out << le.entry << " < ";
            le.Entry->print(out);
        }
        else{
            le.Entry->print(out);
            out << " < " << le.entry;
        }
        return out;
    }

};

#endif
