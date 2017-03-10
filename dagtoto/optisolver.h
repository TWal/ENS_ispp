#ifndef OPTISOLVER_H
#define OPTISOLVER_H

#include "utility.h"

struct OptiSolverResults {
    vvi sols;
    lli cost;
};

OptiSolverResults optiSolve(const Graph& gr);
void printSolutions(const OptiSolverResults& res);

#endif

