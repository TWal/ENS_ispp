#include "optisolver.h"

OptiSolverResults optiSolve(const Graph& gr) {
    OptiSolverResults res;
    vi perm(gr.N);
    FOR(i, gr.N) perm[i] = i;
    do {
        lli cur = getCost(gr.adj, perm);
        if(cur < res.cost) {
            res.sols.clear();
            res.cost = cur;
        }
        if(res.cost == cur) {
            res.sols.pb(perm);
        }
    } while(next_permutation(perm.begin(), perm.end()));
    return res;
}

void printSolutions(const OptiSolverResults& res) {
    printf("Got %lu solutions with cost %lld:\n", res.sols.size(), res.cost);
    for(const vi& perm : res.sols) {
        FOR(i, perm.size()) printf("%lld%c", perm[i], " \n"[i+1 == (lli)perm.size()]);
    }
}
