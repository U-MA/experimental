#ifndef VRPSOLVER_CPP_SIMULATION_H
#define VRPSOLVER_CPP_SIMULATION_H

#include "base_vrp.h"
#include "solution.h"


class Simulator
{
public:
    unsigned int sequential_random_simulation(const BaseVrp& vrp, Solution& solution);

    /* solutionの状態からシミュレーションをcount回行う
     * その中で最も良いコストを返し、その解をsolutionに格納する */
    unsigned int sequential_random_simulation(const BaseVrp& vrp, Solution& solution,
                                            unsigned int count);

    unsigned int random(const BaseVrp& vrp, Solution& solution);

private:
    static const int kInfinity = 1e6;
};

#endif /* VRPSOLVER_CPP_SIMULATION_H */
