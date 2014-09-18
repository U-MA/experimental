#ifndef VRPSOLVER_TOOLS_NODE_DUMP
#define VRPSOLVER_TOOLS_NODE_DUMP

namespace VrpSolver {
    namespace tools {

        namespace detail {
            void node_dump_impl(MctNode *node, int depth, int max_depth);
        }

        // MctNodeから成る木構造をダンプする
        // max_depthで何段目までなのかを指定する.
        // 1であればROOTとその子まで. -1は全てを表示
        void node_dump(MctNode &root, int max_depth = -1) {
            detail::node_dump_impl(&root, 0, max_depth);
        }

        namespace detail {
            void node_dump_impl(MctNode *node, int depth, int max_depth)
            {
                char tab[80] = {};
                for (int i=0; i < 4*depth; ++i) {
                    tab[i] = ' ';
                }

                printf("%snode_id:", tab);
                if (depth == 0) { // root
                    printf("ROOT\n");
                } else {          // others
                    printf("%d\n", node->CustomerId());
                }
                printf("%s  count:%3d,", tab, node->Count());
                printf("num_childs:%3d\n", node->ChildSize());
                if (node->ChildSize() != 0) {
                    if (depth != max_depth) {
                        printf("%s  child:\n", tab);
                        for (int i=0; i < node->ChildSize(); ++i) {
                            node_dump_impl(node->Child(i), depth+1, max_depth);
                        }
                    }
                }
            }
        } // namespace detail

    } // namespace tools
} // namespaceVrpSolver

#endif // VRPSOLVER_TOOLS_NODE_DUMP
