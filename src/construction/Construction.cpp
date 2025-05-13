#include <vector>
#include <unordered_set>
#include <random>
#include <limits>
#include <algorithm>
#include "Construction.hpp"

void Construction::initSolution(const Data &data, Solution &s, std::vector<int> &CL, const int dim)
{
    if (dim < 4)
        throw std::invalid_argument("Falha na construção: Dimensão muito pequena.");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, dim);

    s.sequence.resize(5);
    s.sequence[0] = s.sequence[4] = 1;

    std::unordered_set<int> nums;
    while (nums.size() < 3)
        nums.insert(dis(gen));

    auto it = nums.begin();
    for (size_t pos = 1; pos <= 3; ++pos)
    {
        s.sequence[pos] = *it++;
        if (pos > 0)
        { // Calcula a distância entre nós consecutivos
            s.cost += data.getDistance(s.sequence[pos - 1], s.sequence[pos]);
        }
    }
    // Adiciona a distância do último nó de volta para 1
    s.cost += data.getDistance(s.sequence[3], s.sequence[4]);

    for (int i = 2; i <= dim; ++i)
    {
        if (nums.find(i) == nums.end())
            CL.push_back(i);
    }
}

void Construction::initializeMinDist(const Data &data, const std::vector<int> &sequence, const std::vector<int> &CL, std::vector<double> &min_dist)
{
    for (int node : sequence)
    {
        for (int unvisited : CL)
        {
            double dist = data.getDistance(node, unvisited);
            if (dist < min_dist[unvisited])
            {
                min_dist[unvisited] = dist;
            }
        }
    }
}

int Construction::selectClosestNode(const std::vector<int> &CL, const std::vector<double> &min_dist)
{
    int k = -1;
    double min_val = std::numeric_limits<double>::max();
    for (int node : CL)
    {
        if (min_dist[node] < min_val)
        {
            min_val = min_dist[node];
            k = node;
        }
    }
    return k;
}

void Construction::findBestInsertion(const Data &data, const std::vector<int> &sequence, int k, int &best_pos, double &best_cost)
{
    best_cost = std::numeric_limits<double>::max();
    best_pos = -1;
    for (int a = 0; a < sequence.size() - 1; ++a)
    {
        int i = sequence[a];
        int j = sequence[a + 1];
        double cost = data.getDistance(i, k) + data.getDistance(k, j) - data.getDistance(i, j);
        if (cost < best_cost)
        {
            best_cost = cost;
            best_pos = a;
        }
    }
}

void Construction::updateMinDist(const Data &data, int inserted_node, const std::vector<int> &CL, std::vector<double> &min_dist)
{
    for (int unvisited : CL)
    {
        double dist = data.getDistance(inserted_node, unvisited);
        if (dist < min_dist[unvisited])
        {
            min_dist[unvisited] = dist;
        }
    }
}

void Construction::run(const Data &data, Solution &s)
{
    std::vector<int> CL;
    initSolution(data, s, CL, data.getDimension());
    int dim = data.getDimension();
    std::vector<double> min_dist(dim + 1, std::numeric_limits<double>::max());

    initializeMinDist(data, s.sequence, CL, min_dist);

    while (!CL.empty())
    {
        int k = selectClosestNode(CL, min_dist);
        if (k == -1)
            break; // Should not happen

        // Remove k from CL
        CL.erase(std::remove(CL.begin(), CL.end(), k), CL.end());

        // Find best insertion position
        int best_pos;
        double best_cost;
        findBestInsertion(data, s.sequence, k, best_pos, best_cost);

        // Insert k after position best_pos
        s.sequence.insert(s.sequence.begin() + best_pos + 1, k);
        s.cost += best_cost;

        // Update min_dist for remaining CL
        updateMinDist(data, k, CL, min_dist);
    }
}