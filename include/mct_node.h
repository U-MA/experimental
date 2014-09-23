#ifndef VRPSOLVER_CPP_MCT_NODE_H
#define VRPSOLVER_CPP_MCT_NODE_H

#include "base_node.h"

class MctNode : public BaseNode
{
public:
    MctNode(int customer_id) :
	        is_good_(true),
	        child_(),
                child_size_(),
                count_(0),
                customer_id_(customer_id),
                ave_value_(1000000.0)
    {
        if (customer_id < 0) customer_id_ = 0;
    }

    ~MctNode();

    MctNode* child(unsigned int child_id) const;

    unsigned int child_size() const { return child_size_; }

    unsigned int count() const { return count_; }

    unsigned int customer_id() const { return customer_id_; }
    void count_up() { count_++; }

    double ave_value() const { return ave_value_; }
    void set_ave_value(double value) { ave_value_ = value; }

    void create_child(unsigned int customer_id);
    bool is_leaf() const;
    void update(double value);

    bool is_good_;

private:
    static const int kMaxChildSize = 120;

    MctNode* child_[kMaxChildSize];
    unsigned int child_size_;
    unsigned long count_; // fix int->long; 2014.06.03
    unsigned int customer_id_;
    double ave_value_;
};

#endif /* VRPSOLVER_CPP_MCT_NODE_H */
