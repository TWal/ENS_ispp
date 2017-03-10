#include "uglyInclude.h"
#include "utility.h"
#include "optisolver.h"
#include "random.h"


int main() {
    Graph gr = readGraph(stdin);
#define WHAT 0
#if WHAT==0
    findRandom(gr);
#elif WHAT==1
    OptiSolverResults res = optiSolve(gr);
    printSolutions(res);
#endif
    return 0;
}
