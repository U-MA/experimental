#include <stdio.h>
#include <string.h>

#include "vehicle.h"

void
Vehicle::visit(const BaseVrp& vrp, int customer)
{
    route_[route_length_++] = customer;
    capacity_ += vrp.demand(customer);
    is_visit_[customer-1] = true;
}

unsigned int
Vehicle::compute_cost(const BaseVrp& vrp) const
{
    if (route_length_ == 0) return 0;

    const int depot = 0;
    int i, cost = vrp.cost(depot, route_[0]);
    for (i=1; i < route_length_; i++)
        cost += vrp.cost(route_[i-1], route_[i]);
    cost += vrp.cost(route_[i-1], depot);

    return cost;
}

void
Vehicle::print() const
{
    printf("[%6d] ", capacity_);
    for (int i=0; i < route_length_; i++)
        printf("%4d", route_[i]);
    printf("\n");
}
