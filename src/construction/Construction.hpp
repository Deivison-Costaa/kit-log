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
    void initializeMinDist(const Data &data, const std::vector<int> &sequence, const std::unordered_set<int> &CL, std::vector<double> &min_dist);
    int selectClosestNode(const std::unordered_set<int> &CL, const std::vector<double> &min_dist);
    void findBestInsertion(const Data &data, const std::vector<int> &sequence, int k, int &best_pos, double &best_cost);
    void updateMinDist(const Data &data, int inserted_node, const std::unordered_set<int> &CL, std::vector<double> &min_dist);

    void calculateInsertionCost(const Data &data, const std::vector<int> &sequence, const std::unordered_set<int> &CL, std::vector<InsertionInfo> &insertionInfo);
    void updateConstruction(std::unordered_set<int> &CL, Solution &s, const InsertionInfo &insertionInfo);

    void runNI(const Data &data, Solution &s);
    void runCI(const Data &data, Solution &s);

public:
    void runConstruction(const Data &data, Solution &s, char method);
};