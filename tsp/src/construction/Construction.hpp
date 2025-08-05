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
private:
    void initSolution(const Data &data, Solution &sequence, std::unordered_set<int> &CL, const int dim);
    void calculateInsertionCost(const Data &data, const std::vector<int> &sequence, const std::unordered_set<int> &CL, std::vector<InsertionInfo> &insertionInfo);
    void updateConstruction(std::unordered_set<int> &CL, Solution &s, const InsertionInfo &insertionInfo);

public:
    Solution run(const Data &data);
};