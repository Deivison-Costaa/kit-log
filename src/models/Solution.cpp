#include "Solution.hpp"
#include <iostream>

void Solution::print() const
{
    std::cout << sequence[0] << " -> ";
    for (size_t i = 1; i < sequence.size() - 1; ++i)
    {
        std::cout << sequence[i] << " -> ";
    }
    std::cout << sequence[sequence.size() - 1] << std::endl;
}

int Solution::checkCost(const Data &data) const
{
    double calculatedCost = 0.0;

    for (size_t i = 0; i < sequence.size() - 1; ++i)
    {
        calculatedCost += data.getDistance(sequence[i], sequence[i + 1]);
    }
    return calculatedCost - cost;
}