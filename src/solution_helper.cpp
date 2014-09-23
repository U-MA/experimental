#include "solution_helper.h"

void SolutionHelper::Transition(Solution &solution, const BaseVrp &vrp, unsigned int move)
{
    if (move != 0)
        solution.current_vehicle()->Visit(vrp, move);
    else
        solution.change_vehicle();
}
