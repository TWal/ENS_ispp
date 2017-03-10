#include "uglyInclude.h"
#include "utility.h"
#include "optisolver.h"
#include "random.h"
#include "recuit.h"
#include "heuristics.h"

int main() {
    Graph gr = readGraph(stdin);
#define WHAT 0
#if WHAT==0
    printf("##### INFIX #####\n");
    FOR(i, gr.N) {
        vi infixPerm = infix(gr, i);
        printf("Cost: %lld\tPerm: ", getCost(gr.adj, infixPerm));
        printPerm(infixPerm);
    }
    printf("##### RANDOM #####\n");
    findRandom(gr, 1000000);
    printf("##### SIMULATED ANNEALING #####\n");
    recuit(gr);
#elif WHAT==1
    OptiSolverResults res = optiSolve(gr);
    printSolutions(res);
#endif
    return 0;
}
