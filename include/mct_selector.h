#ifndef VRPSOLVER_CPP_MCT_SELECTOR_H
#define VRPSOLVER_CPP_MCT_SELECTOR_H

#include <vector>

#include "mct_node.h"

namespace Selector
{
    MctNode *Ucb(MctNode& root, std::vector<MctNode*>& visited,
                 double coef = 100.0);

    MctNode *UcbMinus(MctNode& root, std::vector<MctNode*>& visited,
                      int mcts_count, const double coef = 100.0);
}

#endif /* VRPSOLVER_CPP_MCT_SELECTOR_H */
