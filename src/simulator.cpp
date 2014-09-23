#include <stdlib.h>

extern "C"
{
#include "vrp_types.h"
}

#include "mt19937ar.h"
#include "simulator.h"


unsigned int Simulator::sequentialRandomSimulation(const BaseVrp& vrp, Solution& solution)
{
    Vehicle *current_vehicle = solution.current_vehicle();
    int candidates[200], candidate_size;

    while (!solution.is_finish())
    {
        candidate_size = 0;

        /* 次に訪問する顧客の候補を求める */
        for (unsigned int i=1; i <= vrp.CustomerSize(); i++)
        {
            /* 訪問可能であれば候補に追加 */
            if (!solution.is_visit(i) &&
                current_vehicle->Capacity() + vrp.Demand(i) <= vrp.Capacity())
            {
                candidates[candidate_size++] = i;
            }
        }

        if (candidate_size == 0)
        {
            /* 候補がいなければ次の車両へ */
            solution.change_vehicle();
            current_vehicle = solution.current_vehicle();
        }
        else
        {
            /* 候補の中から無作為に１人選ぶ */
            unsigned long r = genrand_int32();
            int customer = candidates[(unsigned int)r % candidate_size];
            current_vehicle->Visit(vrp, customer);
        }
    }

    if (solution.is_feasible())
        return solution.compute_total_cost(vrp);
    else
        return 0;
}

unsigned int
Simulator::sequentialRandomSimulation(const BaseVrp& vrp, Solution& solution, unsigned int count)
{
    unsigned int min_cost = kInfinity;
    Solution better_sol;
    for (unsigned int i=0; i < count; i++)
    {
        Solution solution_copy = solution;
        unsigned int cost = sequentialRandomSimulation(vrp, solution_copy);
        if ((cost != 0) && (cost < min_cost)) {
            better_sol = solution_copy;
            min_cost = cost;
        }
    }
    if (min_cost == kInfinity) return 0;
    solution = better_sol;
    return min_cost;
}

unsigned int Simulator::random(const BaseVrp& vrp, Solution& solution)
{
    return 10000000;
}
