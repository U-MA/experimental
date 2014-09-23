#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "mct_selector.h"

static const int INF = 10000000;


static double
calc_ucb(MctNode *parent, MctNode *child, double coef)
{
    if (child->count() == 0)
        return 100000 + (rand() % 100000);

    return -child->ave_value() +
        coef * sqrt(log(parent->count()) / child->count());
}

MctNode*
Selector::ucb(MctNode& root, std::vector<MctNode *>& visited, double coef)
{
    MctNode *node = &root;
    visited.push_back(node);
    while (!node->is_leaf())
    {
        unsigned int next = 0;
        double max_ucb = -INF;
        for (unsigned int i=0; i < node->child_size(); i++)
        {
            double ucb = calc_ucb(node, node->child(i), coef);
            if (ucb > max_ucb)
            {
                max_ucb = ucb;
                next = i;
            }
        }
        node = node->child(next);
        visited.push_back(node);
    }
    return node;
}

static double
calc_ucb_minus(MctNode *parent, MctNode *child, const double coef)
{
    if (child->count() == 0)
        return - 100000 - (rand() % 100000);

    return child->ave_value() -
        coef * sqrt(log(parent->count()) / child->count());
}

MctNode*
Selector::ucb_minus(MctNode& root, std::vector<MctNode *>& visited, const double coef)
{
    MctNode *node = &root;
    while (!node->is_leaf())
    {
        unsigned int next = 0;
        double min_ucb = INF;
        for (unsigned int i=0; i < node->child_size(); i++)
        {
            // 一度飛ばす
            if (!node->child(i)->is_good_) {
                node->child(i)->is_good_ = true;
                continue;
            }

            double ucb = calc_ucb_minus(node, node->child(i), coef);
            if (ucb < min_ucb)
            {
                min_ucb = ucb;
                next    = i;
            }
        }
        node = node->child(next);
        visited.push_back(node);
    }
    return node;
}
