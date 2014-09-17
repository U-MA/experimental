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
#include "node_dump.h" // for debug


using namespace std;

void usage(char *exe_name);

void
create_childs(const BaseVrp& vrp, Solution& sol, MctNode* node)
{
    for (unsigned int j=0; j <= vrp.CustomerSize(); j++) {
        if (!sol.IsVisit(j) &&
           (sol.CurrentVehicle()->Capacity() + vrp.Demand(j) <= vrp.Capacity()))
            node->CreateChild(j);
    }
    if (node->ChildSize() == 0 && sol.CurrentVehicleId()+1 < vrp.VehicleSize())
        node->CreateChild(0);
}

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

    Solution *sd_list = 0; // とりあえず１要素

    clock_t start = clock();
    while (!solution.IsFinish())
    {
        MctNode root(0);
        for (int i=0; i < mcts_count; i++)
        {
            vector<MctNode *> visited;

            // Selector
            MctNode *node = Selector::UcbMinus(root, visited, ucb_coef);

            Solution solution_copy = solution;

            for (unsigned int j=1; j < visited.size(); j++)
                SolutionHelper::Transition(solution_copy, host_vrp, visited[j]->CustomerId());

            // Expansion
            if (!solution_copy.IsFinish() && (node->Count() >= threshold))
            {
                create_childs(host_vrp, solution_copy, node);

                // sd_listによる先行シミュレーション
                // sd_listの各要素とsolution_copyを比較.solution_copyの要素を全て調べきれば
                // 先行シミュレーションが可能
                if (sd_list) {
                    int next;
                    if (sd_list->find_diff_cus(visited, &next)) {
                        for (int i=0; i < node->ChildSize(); ++i) {
                            if (node->Child(i)->CustomerId() == next) {
                                node->Child(i)->Update(sd_list->ComputeTotalCost(host_vrp));
                                break;
                            }
                        }
                    }
                }

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

            // sd_listへの登録
            // insert here
            if (!sd_list) {
                sd_list = (Solution *)malloc(sizeof(Solution));
                *sd_list = solution_copy;
            } else if (cost < sd_list->ComputeTotalCost(host_vrp)) {
                *sd_list = solution_copy;
            }

            // Backpropagation
            for (unsigned int j=0; j < visited.size(); j++) {
                visited[j]->Update(cost);
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
        VrpSolver::tools::node_dump(root); exit(0);
    }
    clock_t stop = clock();

    if (sd_list) free(sd_list);

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
