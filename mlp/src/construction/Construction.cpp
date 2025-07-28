#include <vector>
#include <unordered_set>
#include <random>
#include <limits>
#include <algorithm>
#include <queue>
#include "Construction.hpp"


Solution Construction::runMLP(const Data &data, std::vector<double> &R)
{

    Solution s;
    s.sequence = std::vector<int>(data.getDimension() + 1, 0);
    s.sequence[0] = 0;
    s.sequence.back() = 0;
    s.cost = 0.0;

    std::unordered_set<int> CL;
    for (int i = 1; i < data.getDimension(); ++i)
    {
        CL.insert(i);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, R.size() - 1);
    double alpha = R[distrib(gen)];

    int position = 1;
    int r = 0;
    
    while (!CL.empty())
    {
        std::vector<std::pair<int, double>> distances;
        distances.reserve(CL.size());
        for (int node : CL)
        {
            double dist = data.getDistance(r, node);
            distances.emplace_back(node, dist);
        }

        std::sort(distances.begin(), distances.end(),
                  [](const auto &a, const auto &b)
                  { return a.second < b.second; });

        size_t rcl_size = static_cast<size_t>(std::ceil(alpha * distances.size()));
        if (rcl_size == 0)
            rcl_size = 1;

        std::vector<int> RCL;
        RCL.reserve(rcl_size);
        for (size_t i = 0; i < rcl_size && i < distances.size(); ++i)
        {
            RCL.push_back(distances[i].first);
        }

        std::uniform_int_distribution<> rcl_distrib(0, RCL.size() - 1);
        int selected_node = RCL[rcl_distrib(gen)];

        s.sequence[position] = selected_node;
        position++;

        r = selected_node;

        CL.erase(selected_node);
    }

    return s;
}