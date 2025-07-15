#include <vector>
#include <unordered_set>
#include <random>
#include <limits>
#include <algorithm>
#include <queue>
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

void Construction::initializeMinDist(const Data &data, const std::vector<int> &sequence, const std::unordered_set<int> &CL, std::vector<double> &min_dist)
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

int Construction::selectClosestNode(const std::unordered_set<int> &CL, const std::vector<double> &min_dist)
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

void Construction::updateMinDist(const Data &data, int inserted_node, const std::unordered_set<int> &CL, std::vector<double> &min_dist)
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

void Construction::runNI(const Data &data, Solution &s)
{
    std::unordered_set<int> CL;
    initSolution(data, s, CL, data.getDimension());

    int dim = data.getDimension();
    // min_dist[i] = distância mínima de i até qualquer nó na solução corrente
    std::vector<double> min_dist(dim + 1, std::numeric_limits<double>::max());
    initializeMinDist(data, s.sequence, CL, min_dist);

    // Enquanto existirem nós não inseridos
    while (!CL.empty())
    {
        // Seleciona o nó mais próximo (NI)
        int k = selectClosestNode(CL, min_dist);
        if (k == -1)
            break; // situação atípica, mas previne loop infinito

        // Remove k do conjunto de candidatos
        CL.erase(k);

        // Encontra a melhor posição de inserção de k na corrente sequência
        int best_pos;
        double best_cost;
        findBestInsertion(data, s.sequence, k, best_pos, best_cost);

        // Insere k na posição e atualiza custo acumulado
        s.sequence.insert(s.sequence.begin() + best_pos + 1, k);
        s.cost += best_cost;

        // Atualiza distâncias mínimas em relação ao nó inserido
        updateMinDist(data, k, CL, min_dist);
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

void Construction::runCI(const Data &data, Solution &s)
{
    std::unordered_set<int> CL;
    initSolution(data, s, CL, data.getDimension());

    std::vector<InsertionInfo> insertionCost;
    insertionCost.reserve((data.getDimension() - 1) * (data.getDimension() - 2)); //evitar realocar memoria

    while (!CL.empty())
    {
        insertionCost.resize((s.sequence.size() - 1) * CL.size());
        calculateInsertionCost(data, s.sequence, CL, insertionCost);

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
}

/**
 * @brief Executa a construção de uma solução TSP usando o método especificado.
 *
 * Esta função escolhe entre Nearest Insertion (NI) e Cheapest Insertion (CI)
 * com base no argumento method.
 *
 * @param data Dados do problema TSP (distancias entre nós).
 * @param s Solução a ser construída (sequência e custo).
 * @param method Char especificando o método: 'n' para Nearest Insertion ou 'c' para Cheapest Insertion.
 * @throws std::invalid_argument Se o método especificado for inválido.
 */
void Construction::runConstruction(const Data &data, Solution &s, char method)
{
    method = std::tolower(static_cast<unsigned char>(method));

    if (method == 'n')
    {
        runNI(data, s);
    }
    else if (method == 'c')
    {
        runCI(data, s);
    }
    else
    {
        throw std::invalid_argument("Método inválido. Use 'n' ou 'c'.");
    }
}