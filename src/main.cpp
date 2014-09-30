#include <cstdlib>
#include <iostream>
#include <vector>
#include <stack>

#include <limits.h>

#include "mt19937ar.h"
#include "host_vrp.h"
#include "mct_node.h"
#include "solution.h"
#include "mct_selector.h"
#include "simulator.h"

#ifdef VRPDEBUG
#include "node_dump.h"
#endif


using namespace std;

void usage(char *exe_name);

double ucb_coef;
int    threshold;
int    simulation_count;

void
transition(Solution &solution, const BaseVrp &vrp, unsigned int move)
{
    if (move != 0)
        solution.current_vehicle()->visit(vrp, move);
    else
        solution.change_vehicle();
}

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

unsigned int
mct_search(const HostVrp& vrp, Solution& sltn, MctNode& node)
{
    if (sltn.is_finish()) {
        unsigned int cost = sltn.compute_total_cost(vrp);
        node.update(cost);
        return cost;
    }

    /* node is NOT a leaf */
    if (node.child_size() > 0) {
        /* select next child with ucb */
        MctNode *next = Selector::ucb_minus(node, ucb_coef);
        transition(sltn, vrp, next->customer_id());
        unsigned int cost = mct_search(vrp, sltn, *next);
        node.update(cost);
        return cost;
    }

    /* node is a leaf */
    if (node.count() >= threshold) {
        create_childs(vrp, sltn, &node);
        /* select next child with ucb */
        MctNode *next = Selector::ucb_minus(node, ucb_coef);
        transition(sltn, vrp, next->customer_id());
        unsigned int cost = mct_search(vrp, sltn, *next);
        node.update(cost);
        return cost;
    }

    Simulator simulator;
    unsigned int cost = simulator.sequential_random_simulation(vrp, sltn, simulation_count);
    node.update(cost);
    return cost;
}

int
main(int argc, char **argv)
{
    if (argc != 6)
        usage(argv[0]);

    const char*  problem_name = argv[1];
    const int    mcts_count   = atoi(argv[2]);

    /* global variables*/
    ucb_coef         = atof(argv[3]);
    threshold        = atoi(argv[4]);
    simulation_count = atoi(argv[5]);

    init_genrand(2014);

    HostVrp  host_vrp(problem_name);
    Solution solution(host_vrp);

    clock_t start = clock();
    while (!solution.is_finish())
    {
        MctNode root(0);
        root.count_up();
        create_childs(host_vrp, solution, &root);
        for (int i=1; i < mcts_count; i++)
        {
            Solution tmp = solution; // solutionを退避
            static_cast<void>(mct_search(host_vrp, solution, root));
            solution = tmp; // solutionの復帰
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
            solution.print();
            printf("root has %u childs\n", root.child_size());
            printf("child's value is %g\n", root.child(0)->ave_value());
            return 1;
        }
        transition(solution, host_vrp, next->customer_id());
    }
    clock_t stop = clock();

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
