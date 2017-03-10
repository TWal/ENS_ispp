#include "random.h"

void findRandom(const Graph& gr) {
    vi perm(gr.N);
    FOR(i, gr.N) perm[i] = i;
    lli bestCost = INF;
    while(true) {
        lli cost = getCost(gr.adj, perm);
        if(cost < bestCost) {
            bestCost = cost;
            printf("Cost: %lld\tPerm: ", cost);
            FOR(i, perm.size()) printf("%lld%c", perm[i], " \n"[i+1 == (lli)perm.size()]);
        }
        random_shuffle(perm.begin(), perm.end());
    }
}
