#include <cstdlib>
#include "mct_node.h"

MctNode::~MctNode()
{
    for (unsigned int i=0; i < child_size_; i++)
        delete(child_[i]);
}

MctNode* MctNode::Child(unsigned int child_id) const
{
    if (child_id < 0 || child_id > kMaxChildSize)
        return NULL;

    if (child_id >= child_size_)
        return NULL;

    return child_[child_id];
}

bool MctNode::IsLeaf() const
{
    return (child_size_ == 0);
}

void MctNode::Update(double value)
{
    count_++;
    ave_value_ = (ave_value_ * (count_-1) + value) / count_;
}

void MctNode::CreateChild(unsigned int customer_id)
{
    child_[child_size_] = new MctNode(customer_id);
    child_size_++;
}
