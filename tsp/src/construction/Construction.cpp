#include <vector>
#include <unordered_set>
#include <random>
#include <limits>
#include <algorithm>
#include "Construction.hpp"

void Construction::initSolution(const Data &data, Solution &s, std::unordered_set<int> &CL, const int dim)
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
            CL.insert(i);
    }
}

void Construction::calculateInsertionCost(const Data &data, const std::vector<int> &sequence, const std::unordered_set<int> &CL, std::vector<InsertionInfo> &insertionCost)
{
    int l = 0;
    for (int a = 0; a < sequence.size() - 1; ++a)
    {
        int i = sequence[a];
        int j = sequence[a + 1];
        for (auto k : CL)
        {
            insertionCost[l].cost = data.getDistance(i, k) + data.getDistance(j, k) - data.getDistance(i, j);
            insertionCost[l].insertedNode = k;
            insertionCost[l].removedEdge = a;
            l++;
        }
    }
}

void Construction::updateConstruction(std::unordered_set<int> &CL, Solution &s, const InsertionInfo &insertionInfo)
{
    auto it = CL.find(insertionInfo.insertedNode);
    if (it != CL.end())
    {
        CL.erase(it); // Erase por iterador (mais eficiente que erase por chave)
    }

    s.sequence.insert(s.sequence.begin() + insertionInfo.removedEdge + 1, insertionInfo.insertedNode);
    s.cost += insertionInfo.cost;
}

Solution Construction::run(const Data &data)
{
    Solution s;
    s.cost = 0;
    std::unordered_set<int> CL;
    initSolution(data, s, CL, data.getDimension());

    std::vector<InsertionInfo> insertionCost;
    insertionCost.reserve((data.getDimension() - 1) * (data.getDimension() - 2)); // evitar realocar memoria

    while (!CL.empty())
    {
        insertionCost.resize((s.sequence.size() - 1) * CL.size());
        calculateInsertionCost(data, s.sequence, CL, insertionCost);

        // double alpha = 0.15;
        double alpha = (double)rand() / RAND_MAX;
        size_t N = static_cast<size_t>(ceil(alpha * insertionCost.size()));
        if (N > insertionCost.size())
            N = insertionCost.size();
        if (N == 0)
            N = 1;

        auto comparator = [](const InsertionInfo &a, const InsertionInfo &b)
        {
            return a.cost < b.cost;
        };

        // separa o vetor entre valores menores | elemento | maiores
        std::nth_element(insertionCost.begin(), insertionCost.begin() + N - 1, insertionCost.end(), comparator);

        int select = rand() % N;
        updateConstruction(CL, s, insertionCost[select]);
    }
    //cout << s.checkCost(data) << endl;
    return s;
}