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

    MctNode* Child(unsigned int child_id) const;

    unsigned int ChildSize() const { return child_size_; }

    unsigned int Count() const { return count_; }

    unsigned int CustomerId() const { return customer_id_; }
    void CountUp() { count_++; }

    double AveValue() const { return ave_value_; }
    void SetAveValue(double value) { ave_value_ = value; }

    void CreateChild(unsigned int customer_id);
    bool IsLeaf() const;
    void Update(double value);

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
