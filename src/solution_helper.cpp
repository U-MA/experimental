#include "solution_helper.h"

void SolutionHelper::Transition(Solution &solution, const BaseVrp &vrp, unsigned int move)
{
    if (move != 0)
        solution.CurrentVehicle()->Visit(vrp, move);
    else
        solution.ChangeVehicle();
}
