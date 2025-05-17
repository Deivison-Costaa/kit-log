#pragma once


#include "Data.h"
#include "Solution.hpp"

class Ils
{

public:
    void run(const Data &data, Solution &s, const int maxIter, const int maxIterIls, char constructionMethod);
    Solution perturb(const Solution &best);
};

