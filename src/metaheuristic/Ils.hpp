#pragma once


#include "Data.h"
#include "Solution.hpp"

class Ils
{
private:
    Solution perturb(const Data &data, const Solution &best);
public:
    void run(const Data &data, Solution &s, const int maxIter, const int maxIterIls, char constructionMethod);
};

