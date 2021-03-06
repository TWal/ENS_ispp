#include "utility.h"
#include <random>

static lli f(lli a, lli b) {
    return abs(a-b);
}

lli getCost(const vvi& graph, const vi& perm) {
    vi position(perm.size());
    FOR(i, perm.size()) {
        position[perm[i]] = i;
    }
    lli sum = 0;
    FOR(i, graph.size()) {
        FOR(j, graph[i].size()) {
            if(i < graph[i][j]) {
                sum += f(position[i], position[graph[i][j]]);
            }
        }
    }
    return sum;
}

Graph readGraph(FILE* fd) {
    Graph res;
    fscanf(fd, "%lld%lld", &res.N, &res.M);
    res.adj.assign(res.N, vi());

    FOR(i, res.M) {
        lli a, b;
        fscanf(fd, "%lld%lld", &a, &b);
        res.adj[a].pb(b);
        res.adj[b].pb(a);
    }
    return res;
}

vi identityPermutation(lli size) {
    vi perm(size);
    FOR(i, size) perm[i] = i;
    return perm;
}

void printPerm(const vi& perm) {
    FOR(i, perm.size()) printf("%lld%c", perm[i], " \n"[i+1 == (lli)perm.size()]);
}

