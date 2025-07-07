#pragma once

#include <vector>
#include <unordered_set>
#include "Solution.hpp"
#include "Data.h"

struct InsertionInfo
{
    int insertedNode;
    int removedEdge;
    double cost;
};


class Construction
{
public:
    Solution runMLP(const Data &data, std::vector<double> &R);
};