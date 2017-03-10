#ifndef UTILITY_H
#define UTILITY_H

#include "uglyInclude.h"

struct Graph {
    lli N;
    lli M;
    vvi adj;
};

lli getCost(const vvi& graph, const vi& perm);
Graph readGraph(FILE* fd);

#endif

