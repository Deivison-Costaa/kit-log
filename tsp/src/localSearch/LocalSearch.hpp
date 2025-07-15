#pragma once 

#include <vector>
#include "Solution.hpp"
#include "Data.h"

class LocalSearch
{
private:
    static bool bestImprovementSwap(const Data &data, Solution &s);
    static bool bestImprovement2Opt(const Data &data, Solution &s);
    static bool bestImprovementOrOpt(const Data &data, Solution &s, const int blockSize);

public:
    void run(const Data &data, Solution &s);
};