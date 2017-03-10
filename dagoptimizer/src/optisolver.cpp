#include "optisolver.h"
#include <algorithm>

OptiSolverResults optiSolve(const Graph& gr) {
    OptiSolverResults res;
    vi perm = identityPermutation(gr.N);
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
        printPerm(perm);
    }
}
