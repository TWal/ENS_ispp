#include "random.h"
#include <algorithm>

void findRandom(const Graph& gr, lli nbIter) {
    vi perm = identityPermutation(gr.N);
    lli bestCost = INF;
    FOR(i, nbIter) {
        lli cost = getCost(gr.adj, perm);
        if(cost < bestCost) {
            bestCost = cost;
            printf("Cost: %lld\tPerm: ", cost);
            printPerm(perm);
        }
        random_shuffle(perm.begin(), perm.end());
    }
}
