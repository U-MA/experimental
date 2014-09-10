#include <cstdlib>
#include <iostream>
#include <vector>

#include <limits.h>

#include "mt19937ar.h"
#include "host_vrp.h"
#include "mct_node.h"
#include "mct_selector.h"
#include "simulator.h"
#include "solution.h"
#include "solution_helper.h"

using namespace std;

void usage(char *exe_name);

int main(int argc, char **argv)
{
    if (argc != 6) usage(argv[0]);

    const char*  problem_name     = argv[1];
    const int    mcts_count       = atoi(argv[2]);
    const double ucb_coef         = atof(argv[3]);
    const int    threshold        = atoi(argv[4]);
    const int    simulation_count = atoi(argv[5]);

    cout << problem_name     << ", "
        << mcts_count       << ", "
        << ucb_coef         << ", "
        << threshold        << ", " 
        << simulation_count << ", ";

    init_genrand(2014);

    HostVrp  host_vrp(problem_name);
    Solution solution(host_vrp);

    clock_t start = clock();
    while (!solution.IsFinish())
    {
        MctNode root(0);
        for (int i=0; i < mcts_count; i++)
        {
            vector<MctNode *> visited;

            // Selector
            MctNode *node = Selector::UcbMinus(root, visited, ucb_coef);

            Solution solution_copy;
            solution.Copy(solution_copy);

            for (unsigned int j=1; j < visited.size(); j++)
                SolutionHelper::Transition(solution_copy, host_vrp, visited[j]->CustomerId());

            // Expansion
            if (!solution_copy.IsFinish() && (node->Count() >= threshold))
            {
                for (unsigned int j=0; j <= host_vrp.CustomerSize(); j++)
                {
                    if (!solution_copy.IsVisit(j) &&
                            (solution_copy.CurrentVehicle()->Capacity() + host_vrp.Demand(j) <=
                             host_vrp.Capacity()))
                        node->CreateChild(j);
                }
                if (solution_copy.CurrentVehicleId()+1 < host_vrp.VehicleSize())
                    node->CreateChild(0);

                visited.pop_back();
                node = Selector::UcbMinus(*node, visited, ucb_coef);

                int move = (*visited.rbegin())->CustomerId();
                SolutionHelper::Transition(solution_copy, host_vrp, move);
            }

            // Simulation
            Simulator simulator;
            unsigned int cost = simulator.sequentialRandomSimulation(host_vrp, solution_copy, simulation_count);

            // 実行可能解が得られなかった
            if (cost == 0) {
                (*visited.rbegin())->is_good_ = false; // 一度選ばれなくする
                continue;
            }
            // Backpropagation
            for (unsigned int j=0; j < visited.size(); j++) {
                visited[j]->CountUp();
                if (visited[j]->AveValue() > cost) {
                    visited[j]->SetAveValue(cost);
                }
            }
        }

        double min_ave_value = 1000000;
        MctNode *next = NULL;

        for (unsigned int i=0; i < root.ChildSize(); i++)
        {
            double ave_value = root.Child(i)->AveValue();
            if ((ave_value >= 1.0) && (ave_value < min_ave_value))
            {
                min_ave_value = ave_value;
                next = root.Child(i);
            }
        }
        if (next == NULL) {
            fprintf(stderr, "next is NULL\n");
            return 1;
        }
        SolutionHelper::Transition(solution, host_vrp, next->CustomerId());
    }
    clock_t stop = clock();

    int cost;
    if (solution.IsFeasible())
        cost = solution.ComputeTotalCost(host_vrp);
    else
        cost = 10000;

    // *** seconds
    cout << (double)(stop - start) / CLOCKS_PER_SEC << ", ";
    //solution.Print();
    cout << cost << endl;
}

void usage(char *exe_name)
{
    cout << "Usage: " << exe_name
        << " <problem_file_path> <mcts_count> <ucb_coef> <threshold> <simulation_count>"
        << endl;
    exit(0);
}
