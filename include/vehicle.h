#ifndef VRPSOLVER_CPP_VEHICLE_H
#define VRPSOLVER_CPP_VEHICLE_H

#include "base_vrp.h"

class Vehicle
{
public:
    Vehicle(void) : route_(), route_length_(0), capacity_(0), is_visit_()
    {
        for (int i=0; i < kMaxSize; i++)
            is_visit_[i] = false;
    };

    unsigned int Capacity() const { return capacity_; }
    bool IsVisit(int customer_id) const { return is_visit_[customer_id-1]; }

    void Visit(const BaseVrp& vrp, int customer);
    unsigned int ComputeCost(const BaseVrp& vrp) const;

    void Print() const;

private:
    static const int kMaxSize = 130;

    int  route_[kMaxSize];
    int  route_length_;
    int  capacity_;
    bool is_visit_[kMaxSize];
};

#endif /* VRPSOLVER_CPP_VEHICLE_H */
