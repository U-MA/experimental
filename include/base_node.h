#ifndef VRPSOLVER_CPP_BASE_NODE_H
#define VRPSOLVER_CPP_BASE_NODE_H

class BaseNode
{
public:
    virtual ~BaseNode() {}

    virtual unsigned int ChildSize() const = 0;
    virtual unsigned int Count() const = 0;
    virtual double AveValue() const = 0;

    virtual bool IsLeaf() const = 0;
    virtual void Update(double value) = 0;
};

#endif /* VRPSOLVER_CPP_BASE_NODE_H */
