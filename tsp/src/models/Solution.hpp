#pragma once

#include <vector>
#include <Data.h>

struct Solution
{
    std::vector<int> sequence;
    double cost;
    
    void print() const;
    int checkCost(const Data &data) const;
};
