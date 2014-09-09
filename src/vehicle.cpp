#include <stdio.h>
#include <string.h>

#include "vehicle.h"

void Vehicle::Visit(const BaseVrp& vrp, int customer)
{
    route_[route_length_++] = customer;
    capacity_ += vrp.Demand(customer);
    is_visit_[customer-1] = true;
}

unsigned int Vehicle::ComputeCost(const BaseVrp& vrp) const
{
    if (route_length_ == 0) return 0;

    const int depot = 0;
    int i, cost = vrp.Cost(depot, route_[0]);
    for (i=1; i < route_length_; i++)
        cost += vrp.Cost(route_[i-1], route_[i]);
    cost += vrp.Cost(route_[i-1], depot);

    return cost;
}

void Vehicle::Print() const
{
    printf("[%6d] ", capacity_);
    for (int i=0; i < route_length_; i++)
        printf("%4d", route_[i]);
    printf("\n");
}
