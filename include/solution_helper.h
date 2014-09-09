#ifndef VRPSOLVER_CPP_SOLUTION_HELPER_H
#define VRPSOLVER_CPP_SOLUTION_HELPER_H

#include "solution.h"
#include "base_vrp.h"

namespace SolutionHelper
{
    void Transition(Solution &solution, const BaseVrp &vrp, unsigned int move);
};

#endif /* VRPSOLVER_CPP_SOLUTION_HELPER_H */
