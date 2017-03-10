#include "random.h"
#include <algorithm>

void findRandom(const Graph& gr) {
    vi perm = identityPermutation(gr.N);
    lli bestCost = INF;
    while(true) {
        lli cost = getCost(gr.adj, perm);
        if(cost < bestCost) {
            bestCost = cost;
            printf("Cost: %lld\tPerm: ", cost);
            printPerm(perm);
        }
        random_shuffle(perm.begin(), perm.end());
    }
}
