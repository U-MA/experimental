#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "mct_selector.h"

static const int INF = 10000000;


static double
calc_ucb(MctNode *parent, MctNode *child, double coef)
{
    if (child->Count() == 0)
        return 100000 + (rand() % 100000);

    return -child->AveValue() +
        coef * sqrt(log(parent->Count()) / child->Count());
}

MctNode*
Selector::ucb(MctNode& root, std::vector<MctNode *>& visited, double coef)
{
    MctNode *node = &root;
    visited.push_back(node);
    while (!node->IsLeaf())
    {
        unsigned int next = 0;
        double max_ucb = -INF;
        for (unsigned int i=0; i < node->ChildSize(); i++)
        {
            double ucb = calc_ucb(node, node->Child(i), coef);
            if (ucb > max_ucb)
            {
                max_ucb = ucb;
                next = i;
            }
        }
        node = node->Child(next);
        visited.push_back(node);
    }
    return node;
}

static double
calc_ucb_minus(MctNode *parent, MctNode *child, const double coef)
{
    if (child->Count() == 0)
        return - 100000 - (rand() % 100000);

    return child->AveValue() -
        coef * sqrt(log(parent->Count()) / child->Count());
}

MctNode*
Selector::ucb_minus(MctNode& root, std::vector<MctNode *>& visited, const double coef)
{
    MctNode *node = &root;
    while (!node->IsLeaf())
    {
        unsigned int next = 0;
        double min_ucb = INF;
        for (unsigned int i=0; i < node->ChildSize(); i++)
        {
            // 一度飛ばす
            if (!node->Child(i)->is_good_) {
                node->Child(i)->is_good_ = true;
                continue;
            }

            double ucb = calc_ucb_minus(node, node->Child(i), coef);
            if (ucb < min_ucb)
            {
                min_ucb = ucb;
                next    = i;
            }
        }
        node = node->Child(next);
        visited.push_back(node);
    }
    return node;
}
