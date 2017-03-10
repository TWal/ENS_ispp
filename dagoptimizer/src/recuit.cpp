#include "recuit.h"
#include <random>

void recuit(const Graph& gr) {
    recuit(gr, identityPermutation(gr.N));
}

const double INITIAL_TEMP = 100.0;
const double TEMP_FACTOR = 0.999999;
const double TEMP_STOP = 1;

double P(lli lastCost, lli newCost, double T) {
    return max((double)1, exp((lastCost-newCost)/T));
}

void recuit(const Graph& gr, vi perm) {
    mt19937 gen;
    uniform_real_distribution<double> dist01;
    uniform_int_distribution<lli> distint(0, gr.N-1);
    double T = INITIAL_TEMP;
    lli lastCost = getCost(gr.adj, perm);
    lli bestCost = lastCost;
    lli stepCount = 0;
    while(T > TEMP_STOP) {
        if(stepCount%50000 == 0) {
            printf("BestCost: %lld\tCost:%lld\tT:%lf\r", bestCost, lastCost, T);
        }
        lli n1 = distint(gen);
        lli n2 = distint(gen);
        swap(perm[n1], perm[n2]);
        lli newCost = getCost(gr.adj, perm);
        if(P(lastCost, newCost, T) >= dist01(gen)) {
            lastCost = newCost;
            if(newCost < bestCost) {
                bestCost = newCost;
                printf("Cost: %lld\tPerm: ", newCost);
                printPerm(perm);
            }
        } else {
            swap(perm[n1], perm[n2]);
        }
        T *= TEMP_FACTOR;
        stepCount += 1;
    }
}

