
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <array>
#include <cstring>
#include "Blocks.h"
using namespace std;

#include "tree.cpp"

ostream& show(ostream& out, leaf_t* lf);
ostream& show(ostream& out, node_t* nd);
ostream& show(ostream& out, tree_t* tr);

ostream& show(ostream& out, leaf_t* lf) {
    leaf_t::match<void>(lf, [&] (int i) { out << i; });
    return out;
}

ostream& show(ostream& out, node_t* nd) {
    node_t::match<void>(nd, [&] (tree_t* t1, tree_t *t2) {
            out << "(";
            show(out,t1);
            out << ") (";
            show(out,t2);
            out << ")";
            });
    return out;
}

ostream& show(ostream& out, tree_t* tr) {
    tree_t::match<void>(tr,
            [&] (node_t* nd) { show(out, nd); },
            [&] (leaf_t* lf) { show(out, lf); });
    return out;
}

int main() {
    int input;
    vector<tree_t*> pile;
    init_blocks();

    while(cin >> input) {
        if(input != -42) {
            pile.push_back(tree_t::build_leaf(leaf_t::build(input)));
        } else if(pile.size() >= 2) {
            tree_t* t1 = pile.back(); pile.pop_back();
            tree_t* t2 = pile.back(); pile.pop_back();
            pile.push_back(tree_t::build_node(node_t::build(t1, t2)));
        } else {
            cerr << "Invalid code" << endl;
            return 1;
        }

        for(size_t i = 0; i < pile.size(); ++i) {
            cout << i << ". ";
            show(cout, pile[i]);
            cout << endl;
        }
    }

    return 0;
}

