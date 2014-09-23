#ifndef VRPSOLVER_CPP_BASE_VRP_H
#define VRPSOLVER_CPP_BASE_VRP_H

class BaseVrp
{
public:
    virtual ~BaseVrp() {};

    virtual unsigned int customer_size()  const = 0;
    virtual unsigned int vehicle_size()   const = 0;
    virtual unsigned int capacity()      const = 0;
    virtual unsigned int cost(int v0, int v1) const = 0;
    virtual unsigned int demand(int v)        const = 0;
};

#endif /* VRPSOLVER_CPP_BASE_VRP_H */
