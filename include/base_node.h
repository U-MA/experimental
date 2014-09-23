#ifndef VRPSOLVER_CPP_BASE_NODE_H
#define VRPSOLVER_CPP_BASE_NODE_H

class BaseNode
{
public:
    virtual ~BaseNode() {}

    virtual unsigned int child_size() const = 0;
    virtual unsigned int count() const = 0;
    virtual double ave_value() const = 0;

    virtual bool is_leaf() const = 0;
    virtual void update(double value) = 0;
};

#endif /* VRPSOLVER_CPP_BASE_NODE_H */
