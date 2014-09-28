#ifndef VRPSOLVER_TOOLS_NODE_DUMP
#define VRPSOLVER_TOOLS_NODE_DUMP

#include <iostream>

#include "mct_node.h"

namespace VrpSolver {
    namespace tools {

        namespace detail {
            void node_dump_impl(FILE* fp, MctNode *node, int depth, int max_depth);
        }

        // MctNodeから成る木構造をダンプする
        // max_depthで何段目までなのかを指定する.
        // 1であればROOTとその子まで. -1は全てを表示
        void node_dump(FILE* fp, MctNode &root, int max_depth = -1) {
            detail::node_dump_impl(fp, &root, 0, max_depth);
        }

        namespace detail {
            void node_dump_impl(FILE* fp, MctNode *node, int depth, int max_depth)
            {
                char tab[80] = {};
                for (int i=0; i < 4*depth; ++i) {
                    tab[i] = ' ';
                }

                fprintf(fp, "%snode_id:", tab);
                if (depth == 0) { // root
                    fprintf(fp, "ROOT\n");
                } else {          // others
                    fprintf(fp, "%d\n", node->customer_id());
                }
                fprintf(fp, "%s  count:%3d,", tab, node->count());
                fprintf(fp, "num_childs:%3d\n", node->child_size());
                if (node->child_size() != 0) {
                    if (depth != max_depth) {
                        fprintf(fp, "%s  child:\n", tab);
                        for (int i=0; i < node->child_size(); ++i) {
                            node_dump_impl(fp, node->child(i), depth+1, max_depth);
                        }
                    }
                }
            }
        } // namespace detail

    } // namespace tools
} // namespaceVrpSolver

#endif // VRPSOLVER_TOOLS_NODE_DUMP
