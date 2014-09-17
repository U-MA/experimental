#ifndef VRPSOLVER_TOOLS_NODE_DUMP
#define VRPSOLVER_TOOLS_NODE_DUMP

namespace VrpSolver {
    namespace tools {

        namespace detail {
            void node_dump_impl(MctNode *node, int level);
        }

        // MctNodeから成る木構造をダンプする
        void node_dump(MctNode &root) {
            detail::node_dump_impl(&root, 0);
        }

        namespace detail {
            void node_dump_impl(MctNode *node, int level)
            {
                char tab[50] = {};
                for (int i=0; i < level*2; ++i) {
                    tab[i] = ' ';
                }

                printf("%snode_id: ", tab);
                if (level == 0) { // root
                    printf("ROOT\n");
                } else {          // others
                    printf("%d\n", node->CustomerId());
                    printf("%scount: %d\n", tab, node->Count());
                }
                printf("%snum_childs: %d\n", tab, node->ChildSize());
                if (node->ChildSize() != 0) {
                    printf("%schild:\n", tab);
                    for (int i=0; i < node->ChildSize(); ++i) {
                        node_dump_impl(node->Child(i), level+1);
                    }
                }
            }
        } // namespace detail

    } // namespace tools
} // namespaceVrpSolver

#endif // VRPSOLVER_TOOLS_NODE_DUMP
