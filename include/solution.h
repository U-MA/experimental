#ifndef VRPSOLVER_CPP_SOLUTION_H
#define VRPSOLVER_CPP_SOLUTION_H

#include "base_vrp.h"
#include "vehicle.h"

class Solution
{
public:
    Solution() : vehicles_(),
                 current_vehicle_id_(0),
                 customer_size_(0),
                 vehicle_size_(0) {}

    Solution(const BaseVrp& vrp) : vehicles_(),
                                   current_vehicle_id_(0),
                                   customer_size_(vrp.CustomerSize()),
                                   vehicle_size_(vrp.VehicleSize()) {}

    void Copy(Solution& solution_copy) const;

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

private:
    static const int kMaxVehicleSize = 20;

    Vehicle vehicles_[kMaxVehicleSize];
    unsigned int current_vehicle_id_;
    unsigned int customer_size_;
    unsigned int vehicle_size_;
};

#endif /* VRPSOLVER_CPP_SOLUTION_H */
