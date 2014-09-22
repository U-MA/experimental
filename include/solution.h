#ifndef VRPSOLVER_CPP_SOLUTION_H
#define VRPSOLVER_CPP_SOLUTION_H

#include <vector>

#include "base_vrp.h"
#include "vehicle.h"
#include "mct_node.h"

class Solution
{
public:
    // constractors
    Solution() : vehicles_(),
                 current_vehicle_id_(0),
                 customer_size_(0),
                 vehicle_size_(0) {}

    Solution(const BaseVrp& vrp) : vehicles_(),
                                   current_vehicle_id_(0),
                                   customer_size_(vrp.CustomerSize()),
                                   vehicle_size_(vrp.VehicleSize()) {}

    // copy constractor
    Solution(const Solution& sol)
        : vehicles_(),
          current_vehicle_id_(sol.current_vehicle_id_),
          customer_size_(sol.customer_size_),
          vehicle_size_(sol.vehicle_size_)
    {
        for (int i=0; i < kMaxVehicleSize; ++i)
            vehicles_[i] = sol.vehicles_[i];
    }

    // operator
    Solution& operator=(const Solution& sol)
    {
        for (int i=0; i < kMaxVehicleSize; ++i)
            vehicles_[i] = sol.vehicles_[i];

        current_vehicle_id_ = sol.current_vehicle_id_;
        customer_size_      = sol.customer_size_;
        vehicle_size_       = sol.vehicle_size_;
    }


    // functions

    /* 現在走行している車両を取得
     * privateなメンバ変数へのポインタを返すので
     * いい設計とは思えないが、このままでいく */
    Vehicle *CurrentVehicle();

    /* 現在走行している車両の番号を取得
     * 最初の番号は0 */
    unsigned int CurrentVehicleId() const
    {
        return current_vehicle_id_;
    }

    /* 現在走行しているvehicleを変更 */
    void ChangeVehicle();

    /* solutionが適切なものであればtrue
     * 例えば全ての顧客を訪問していなければ適切なsolutionでないので
     * falseを返す */
    bool IsFeasible() const;

    /* 終了状態であればtrue */
    bool IsFinish() const;

    /* customer_idを訪問していればtrue */
    bool IsVisit(int customer_id) const;

    /* 全てのvehicleのコストを返す */
    unsigned int ComputeTotalCost(const BaseVrp& vrp) const;

    /* 配送ルートを出力 */
    void Print() const;

    /* visitedを見て行き、最初に出現した異なる顧客を返す
     * 引数のvisitedを全て見たときには、第３引数にtrue
     * そうでないときはfalseが入る */
    bool find_diff_cus(const std::vector<MctNode*>& visited, int *next) const;

    /* solutionから*thisに遷移可能であればtrue, そうでなければfalse */
    bool is_derivative_of(const Solution& solution) const;

private:
    static const int kMaxVehicleSize = 20;

    Vehicle vehicles_[kMaxVehicleSize];
    unsigned int current_vehicle_id_;
    unsigned int customer_size_;
    unsigned int vehicle_size_;
};

#endif /* VRPSOLVER_CPP_SOLUTION_H */
