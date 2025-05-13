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
    