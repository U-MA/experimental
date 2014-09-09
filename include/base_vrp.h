#ifndef VRPSOLVER_CPP_BASE_VRP_H
#define VRPSOLVER_CPP_BASE_VRP_H

class BaseVrp
{
public:
    virtual ~BaseVrp() {};

    virtual unsigned int CustomerSize()  const = 0;
    virtual unsigned int VehicleSize()   const = 0;
    virtual unsigned int Capacity()      const = 0;
    virtual unsigned int Cost(int v0, int v1) const = 0;
    virtual unsigned int Demand(int v)        const = 0;
};

#endif /* VRPSOLVER_CPP_BASE_VRP_H */
