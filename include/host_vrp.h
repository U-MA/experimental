#ifndef VRPSOLVER_CPP_HOST_VRP_H
#define VRPSOLVER_CPP_HOST_VRP_H

extern "C"
{
#include "vrp_macros.h"
#include "vrp_types.h"
}

#include "base_vrp.h"

class HostVrp : public BaseVrp
{
public:
    HostVrp();
    HostVrp(const char *file_path);
    ~HostVrp();

    void create(const char *file_path);

    const char* name()        const;
    unsigned int customer_size()  const { return vrp_->vertnum-1; }
    unsigned int vehicle_size()   const { return vrp_->numroutes; }
    unsigned int capacity()       const { return vrp_->capacity; }
    unsigned int cost(int v0, int v1) const { return vrp_->dist.cost[INDEX(v0, v1)]; }
    unsigned int demand(int v)        const { return vrp_->demand[v]; }

private:
    vrp_problem *vrp_;
};

#endif /* VRPSOLVER_CPP_HOST_VRP_H */
