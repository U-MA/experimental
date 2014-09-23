#include <cstdlib>
#include <iostream>
#include <vector>
#include <stack>

#include <limits.h>

#include "mt19937ar.h"
#include "host_vrp.h"
#include "mct_node.h"
#include "mct_selector.h"
#include "simulator.h"
#include "solution.h"
#include "solution_helper.h"

#ifdef VRPDEBUG
#include "node_dump.h"
#endif


using namespace std;

void usage(char *exe_name);

void
create_childs(const BaseVrp& vrp, Solution& sol, MctNode* node)
{
    for (unsigned int j=0; j <= vrp.customer_size(); j++) {
        if (!sol.is_visit(j) &&
           (sol.current_vehicle()->capacity() + vrp.demand(j) <= vrp.capacity()))
            node->create_child(j);
    }
    if (node->child_size() == 0 && sol.current_vehicle_id()+1 < vrp.vehicle_size())
        node->create_child(0);
}

// rootを根とするモンテカルロ木をUCB値に従い探索
// 訪問したノードはvisitedに格納される
MctNode*
traverse_tree(MctNode& root, const BaseVrp& vrp, double ucb_coef, 
              Solution& solution, vector<MctNode*>& visited)
{
    MctNode* node = Selector::ucb_minus(root, visited, ucb_coef);
    for (unsigned int j=0; j < visited.size(); ++j)
        SolutionHelper::transition(solution, vrp, visited[j]->customer_id());
    return node;
}

int
main(int argc, char **argv)
{
    if (argc != 6)
        usage(argv[0]);

    const char*  problem_name     = argv[1];
    const int    mcts_count       = atoi(argv[2]);
    const double ucb_coef         = atof(argv[3]);
    const int    threshold        = atoi(argv[4]);
    const int    simulation_count = atoi(argv[5]);

    init_genrand(2014);

    HostVrp  host_vrp(problem_name);
    Solution solution(host_vrp);

    Solution *sd_list = 0;

    clock_t start = clock();
    while (!solution.is_finish())
    {
        MctNode root(0);
        root.count_up();
        create_childs(host_vrp, solution, &root);
        for (int i=0; i < mcts_count; i++)
        {
            // backpropagationのため、訪問したノードを記憶
            vector<MctNode *> visited;

            Solution solution_copy = solution;

            MctNode* node = traverse_tree(root, host_vrp, ucb_coef, solution_copy, visited);

            // Expansion
            if (!solution_copy.is_finish() && (node->count() >= threshold))
            {
                create_childs(host_vrp, solution_copy, node);

                if (sd_list) {
                    Solution tmp = solution_copy; // solution_copyを退避
                    for (int i=0; i < node->child_size(); ++i) {
                        int next = node->child(i)->customer_id();
                        SolutionHelper::transition(solution_copy, host_vrp, next);
                        if (sd_list->is_derivative_of(solution_copy)) {
                            int cost = sd_list->compute_total_cost(host_vrp);
                            node->child(i)->update(cost);
                        }
                        solution_copy = tmp; // solution_copyを復帰
                    }
                }

                node = Selector::ucb_minus(*node, visited, ucb_coef);

                int move = (*visited.rbegin())->customer_id();
                SolutionHelper::transition(solution_copy, host_vrp, move);
            }

            // Simulation
            Simulator simulator;
            unsigned int cost = simulator.sequential_random_simulation(host_vrp, solution_copy, simulation_count);

            // 実行可能解が得られなかった
            if (cost == 0) {
                (*visited.rbegin())->is_good_ = false; // 一度選ばれなくする
                continue;
            }

            if (!sd_list) {
                sd_list = (Solution *)malloc(sizeof(Solution));
                *sd_list = solution_copy;
            } else if (cost < sd_list->compute_total_cost(host_vrp)) {
                *sd_list = solution_copy;
            }

            // Backpropagation
            for (unsigned int j=0; j < visited.size(); j++) {
                visited[j]->update(cost);
            }
            root.count_up();

        }

        double min_ave_value = 1000000;
        MctNode *next = NULL;

        for (unsigned int i=0; i < root.child_size(); i++)
        {
            double ave_value = root.child(i)->ave_value();
            if ((ave_value >= 1.0) && (ave_value < min_ave_value))
            {
                min_ave_value = ave_value;
                next = root.child(i);
            }
        }
        if (next == NULL) {
            fprintf(stderr, "next is NULL\n");
            return 1;
        }
        SolutionHelper::transition(solution, host_vrp, next->customer_id());
    }
    clock_t stop = clock();

    if (sd_list) free(sd_list);

    int cost;
    if (solution.is_feasible())
        cost = solution.compute_total_cost(host_vrp);
    else
        cost = 10000;


    cout << problem_name     << ", "
         << mcts_count       << ", "
         << ucb_coef         << ", "
         << threshold        << ", " 
         << simulation_count << ", "
         << (double)(stop - start) / CLOCKS_PER_SEC << ", "
         << cost << endl;
}

void
usage(char *exe_name)
{
    cout << "Usage: " << exe_name
        << " <problem_file_path> <mcts_count> <ucb_coef> <threshold> <simulation_count>"
        << endl;
    exit(0);
}
