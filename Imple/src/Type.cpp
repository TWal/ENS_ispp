#include "Type.h"
#include <algorithm>

using namespace std;

std::ostream& operator << (std::ostream& out,const FullType& type){
    out << "type " << type.name << " = ";
    for(auto c : type.content){
        out << " |" << c.first;
        for(auto l : c.second){
            out <<" "<< l.name;
            if(l.pointedType){
                out << "%";
            }
        }
    }
    return out;
}

void fillLink(std::vector<FullType>& fts){
    std::map<std::string,int> n2ind;
    for(size_t i = 0 ; i < fts.size(); ++i){
        n2ind[fts[i].name] = i;
    }
    for(FullType& ft : fts){
        for(pair<const string,std::vector<LeafType>>& p : ft.content){
            for(LeafType& lt : p.second){
                if (n2ind.count(lt.name)){
                    lt.pointedType = &fts[n2ind[lt.name]];
                }
                else lt.pointedType = nullptr;
            }
        }
    }
}

void Codegen::protoBuilder(std::ostream& out,std::string name,
                  const std::pair<std::string,std::vector<LeafType>>& p){
    out <<"\tstatic "<< name <<"* build_"<<p.first << "(";
    for(size_t i = 0 ; i < p.second.size() ; ++i){
        if(i) out << ",";
        auto lt = p.second[i];
        if(lt.pointedType) out << lt.name << "* m" << i;
        else out <<lt.name << " m" << i;
    }
    out << ");"<<endl;

}
void Codegen::protoMatch(std::ostream& out, const FullType& ft){
    out << "\ttemplate<typename Ret";
    for(auto p : ft.content){
        out << ", typename F"<< p.first;
    }
    out << ">" <<endl;
    out << "\tRet match(";
    bool first = true;
    for(auto p : ft.content){
        if(first) first = false;
        else out << ", ";
        out << "const F"<< p.first << "& f"<<p.first;
    }
    out << ");"<<endl;
}

void Codegen::headBuilder(std::ostream& out, std::string name,
                 const std::pair<std::string,std::vector<LeafType>>& p){
    out << name <<"* " << name<<"::build_"<<p.first << "(";
    for(size_t i = 0 ; i < p.second.size() ; ++i){
        if(i) out << ",";
        auto lt = p.second[i];
        if(lt.pointedType) out << lt.name << "* m" << i;
        else out <<lt.name << " m" << i;
    }
    out << "){"<<endl;
}
void Codegen::headMatch(std::ostream& out, const FullType& ft){
    out << "template<typename Ret";
    for(auto p : ft.content){
        out << ", typename F"<< p.first;
    }
    out << ">" <<endl;
    out << "Ret " << ft.name <<"::match(";
    bool first = true;
    for(auto p : ft.content){
        if(first) first = false;
        else out << ", ";
        out << "const F"<< p.first << "& f"<<p.first;
    }
    out << "){"<<endl;
}
