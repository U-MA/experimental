#include <iostream>

#include "solution.h"

Vehicle *Solution::CurrentVehicle()
{
    return &vehicles_[current_vehicle_id_];
}

void Solution::ChangeVehicle()
{
    ++current_vehicle_id_;
}

bool Solution::IsFeasible() const
{
    for (unsigned int i=1; i <= customer_size_; i++)
    {
        for (unsigned int j=0; j <= current_vehicle_id_; j++)
        {
            if (vehicles_[j].IsVisit(i))
                break;
            if (j == current_vehicle_id_)
                return false;
        }
    }
    return true;
}

bool Solution::IsFinish() const
{
    /* 用意されている車両を使いきった */
    /*
    if (current_vehicle_id_ >= vehicle_size_)
        return true;
        */

    /* 全ての顧客を訪問したかの確認 */
    /*
    return IsFeasible();
    */
    return IsFeasible() || (current_vehicle_id_ >= vehicle_size_);
}

bool Solution::IsVisit(int customer_id) const
{
    for (unsigned int i=0; i <= current_vehicle_id_; i++)
        if (vehicles_[i].IsVisit(customer_id))
            return true;
    return false;
}

unsigned int Solution::ComputeTotalCost(const BaseVrp& vrp) const
{
    int total_cost = 0;
    for (unsigned int i=0; i <= current_vehicle_id_; i++)
        total_cost += vehicles_[i].ComputeCost(vrp);
    return total_cost;
}

void Solution::Print() const
{
    for (unsigned int i=0; i <= current_vehicle_id_; i++)
        vehicles_[i].Print();
}

bool
Solution::is_derivative_of(const Solution& sol) const
{
    for (int i=0; i < sol.vehicle_size_; ++i) {
        for (int j=0; j < sol.vehicles_[i].size(); ++j) {
            if (sol.vehicles_[i].get(j) != vehicles_[i].get(j))
                return false;
        }
    }
    return true;
}
