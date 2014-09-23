#ifndef VRPSOLVER_CPP_VEHICLE_H
#define VRPSOLVER_CPP_VEHICLE_H

#include "base_vrp.h"

class Vehicle
{
public:
    typedef int *       iterator;
    typedef const int * const_iterator;

    Vehicle(void) : route_(), route_length_(0), capacity_(0), is_visit_()
    {
        for (int i=0; i < kMaxSize; i++)
            is_visit_[i] = false;
    };

    unsigned int capacity() const { return capacity_; }
    bool is_visit(int customer_id) const { return is_visit_[customer_id-1]; }

    void visit(const BaseVrp& vrp, int customer);
    unsigned int compute_cost(const BaseVrp& vrp) const;

    int get(int i) const
    {
        if (i < 0 || i > route_length_)
            return -1;

        return route_[i];
    }

    int size() const
    {
        return route_length_;
    }

    void print() const;

    iterator begin() { return route_; }
    iterator end()   { return &route_[route_length_]; }
    const_iterator cbegin() const
    { return route_; }
    const_iterator cend() const
    { return &route_[route_length_]; }

private:
    static const int kMaxSize = 130;

    int  route_[kMaxSize];
    int  route_length_;
    int  capacity_;
    bool is_visit_[kMaxSize];
};

#endif /* VRPSOLVER_CPP_VEHICLE_H */
