
#include <iostream>
#include <vector>
#include <functional>
using namespace std;

#include "Blocks.h"
#include "tree.tpp"

ostream& operator<<(ostream& out,tree& tr) {
    tr.match<void>(
        [&] (int i) {out << i; },
        [&] (tree* left,tree* right) {out << "(" << *left << ")(" << *right<<")"; });
    return out;
}

void recur_free(tree* ptr) {
    ptr->match<void>(
            [] (int i) { },
            [] (tree* left, tree* right) { recur_free(left); recur_free(right); });
    block_free<block1>((char*)ptr, sizeof(tree));
}

int main() {
    int input;
    vector<tree*> pile;

    while(cin >> input) {
        if(input == -1 && pile.size() >= 1) {
            recur_free(pile.back());
            pile.pop_back();
        } else if(input != -42) {
            pile.push_back(tree::build_leaf(input));
        } else if(pile.size() >= 2) {
            tree* t1 = pile.back(); pile.pop_back();
            tree* t2 = pile.back(); pile.pop_back();
            pile.push_back(tree::build_node(t1, t2));
        } else {
            cerr << "Invalid code" << endl;
            return 1;
        }

        for(size_t i = 0; i < pile.size(); ++i) {
            cout << i << ". " << *pile[i] << endl;
        }
    }

    return 0;
}

