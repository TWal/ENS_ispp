#include "uglyInclude.h"
#include "utility.h"
#include "optisolver.h"
#include "random.h"
#include "recuit.h"

int main() {
    Graph gr = readGraph(stdin);
#define WHAT 1
#if WHAT==0
    findRandom(gr);
#elif WHAT==1
    recuit(gr);
#elif WHAT==2
    OptiSolverResults res = optiSolve(gr);
    printSolutions(res);
#endif
    return 0;
}
