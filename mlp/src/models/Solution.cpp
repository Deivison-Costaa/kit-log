#include "Solution.hpp"
#include <iostream>

void Solution::print() const
{
    std::cout << sequence[0] - 1 << " -> ";
    for (size_t i = 1; i < sequence.size() - 1; ++i)
    {
        std::cout << sequence[i] - 1 << " -> ";
    }
    std::cout << sequence[sequence.size() - 1] - 1 << std::endl;
}

int Solution::checkCost(const Data &data) const
{
    double calculatedCost = 0.0;
    double travelTime = 0.0;

    for (size_t i = 1; i < sequence.size() - 1; ++i)
    {
        travelTime += data.getDistance(sequence[i - 1], sequence[i]);
        calculatedCost += travelTime;
    }
    return calculatedCost - cost;
}


std::ostream &operator<<(std::ostream &os, const Solution &sol)
{
    os << "Sequence: ";
    for (size_t i = 0; i < sol.sequence.size(); ++i)
    {
        os << sol.sequence[i];
        if (i < sol.sequence.size() - 1)
            os << " -> ";
    }
    os << "\nCost: " << sol.cost;
    return os;
}
