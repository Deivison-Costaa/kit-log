#include "LocalSearch.hpp"
#include <stdlib.h>
#include <algorithm>
#include <cassert>
#include "Subsequence.hpp"

bool LocalSearch::bestImprovementSwap(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    double bestDelta = 0.0;
    int bestI = -1, bestJ = -1;
    int N = s.sequence.size() - 2; // Fica mais organizado se definir um N na hora de concatenar

    for (int i = 1; i <= N; ++i)
    {
        for (int j = i + 1; j <= N; ++j)
        {
            // cout << "entrou" << endl;
            Subsequence sigma = subseq_matrix[0][i - 1];
            sigma = Subsequence::Concatenate(sigma, subseq_matrix[j][j], data);
            if (j >= i + 2)
            {
                sigma = Subsequence::Concatenate(sigma, subseq_matrix[i + 1][j - 1], data); // meio (caso exista)
            }
            sigma = Subsequence::Concatenate(sigma, subseq_matrix[i][i], data);
            if (j + 1 <= N)
            {
                sigma = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][N], data);
            }
            double C_Q = sigma.C;
            double delta = C_Q - s.cost;
            if (delta < bestDelta)
            {
                bestDelta = delta;
                bestI = i;
                bestJ = j;
            }
        }
    }

    if (bestDelta < 0)
    {
        std::swap(s.sequence[bestI], s.sequence[bestJ]);
        s.cost += bestDelta;
        // UpdateAllSubseq(s, data, subseq_matrix, bestI, bestJ);
        UpdateAllSubseq(s, data, subseq_matrix);
        assert(s.checkCost(data) == 0);
        return true;
    }
    return false;
}

bool LocalSearch::bestImprovement2Opt(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    double bestDelta = 0.0;
    int bestI = -1, bestJ = -1;
    int N = s.sequence.size() - 2;

    for (int i = 0; i <= N; ++i)
    {
        for (int j = i + 2; j <= N; ++j)
        {
            // Construir a nova sequência: [0,i] + [j,i+1] (invertido) + [j+1,N]
            Subsequence sigma = subseq_matrix[0][i - 1];                                // De 0 até i
            sigma = Subsequence::Concatenate(sigma, subseq_matrix[j][i], data); // De j até i+1 (invertido)
            if (j + 1 <= N)
            {
                sigma = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][N], data); // De j+1 até N
            }
            double C_Q = sigma.C;
            double delta = C_Q - s.cost;
            if (delta < bestDelta)
            {
                bestDelta = delta;
                bestI = i;
                bestJ = j;
            }
        }
    }

    if (bestDelta < 0)
    {
        std::reverse(s.sequence.begin() + bestI, s.sequence.begin() + bestJ);
        s.cost += bestDelta;
        // UpdateAllSubseq(s, data, subseq_matrix, bestI, bestJ);
        UpdateAllSubseq(s, data, subseq_matrix);
        assert(s.checkCost(data) == 0);
        return true;
    }
    return false;
}

bool LocalSearch::bestImprovementOrOpt(const Data &data, Solution &s, const int blockSize, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    double bestDelta = 0.0;
    int bestI = -1, bestJ = -1;
    int N = s.sequence.size() - 2; // Número de clientes

    for (int i = 1; i <= N - blockSize + 1; ++i)
    {
        for (int j = 0; j <= N; ++j)
        {
            // Evitar reinserções inválidas
            if (j >= i - 1 && j < i + blockSize)
                continue; // Não reinserir no mesmo bloco ou adjacente
            if (j == i - 1 && blockSize == 1)
                continue;                                // Evitar swap quando blockSize = 1
            Subsequence sigma = subseq_matrix[0][i - 1]; // De 0 até i-1
            if (j >= i + blockSize)
            {
                // Se j está após o bloco removido, concatenar [i+blockSize, j]
                sigma = Subsequence::Concatenate(sigma, subseq_matrix[i + blockSize][j], data);
            }
            else if (j >= i)
            {
                // Se j está dentro do intervalo removido, pular (já tratado pela condição continue)
                continue;
            }
            else
            {
                // Se j está antes de i, concatenar [j+1, i-1]
                if (j + 1 <= i - 1)
                {
                    sigma = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][i - 1], data);
                }
            }
            // Concatenar o bloco [i, i+blockSize-1]
            sigma = Subsequence::Concatenate(sigma, subseq_matrix[i][i + blockSize - 1], data);
            // Concatenar de j+1 até N (se aplicável)
            if (j + 1 <= N)
            {
                sigma = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][N], data);
            }
            double C_Q = sigma.C;
            double delta = C_Q - s.cost;
            if (delta < bestDelta)
            {
                bestDelta = delta;
                bestI = i;
                bestJ = j;
            }
        }
    }

    if (bestDelta < 0)
    {
        // Extrair o bloco
        std::vector<int> block(s.sequence.begin() + bestI, s.sequence.begin() + bestI + blockSize);
        // Remover o bloco
        s.sequence.erase(s.sequence.begin() + bestI, s.sequence.begin() + bestI + blockSize);
        // Ajustar posição de inserção
        int insertPos = (bestJ >= bestI) ? bestJ - blockSize + 1 : bestJ + 1;
        // Inserir o bloco na nova posição
        s.sequence.insert(s.sequence.begin() + insertPos, block.begin(), block.end());
        s.cost += bestDelta;
        // UpdateAllSubseq(s, data, subseq_matrix, bestI, bestJ);
        UpdateAllSubseq(s, data, subseq_matrix);
        assert(s.checkCost(data) == 0);
        return true;
    }
    return false;
}

void LocalSearch::run(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    UpdateAllSubseq(s, data, subseq_matrix);
    std::vector<int> NL = {1};
    bool improved = false;

    while (!NL.empty())
    {
        int n = rand() % NL.size();

        switch (NL[n])
        {
        case 1:
            improved = bestImprovementSwap(data, s, subseq_matrix);
            break;
        case 2:
            improved = bestImprovement2Opt(data, s, subseq_matrix);
            break;
        case 3:
            improved = bestImprovementOrOpt(data, s, 1, subseq_matrix);
            break;
        case 4:
            improved = bestImprovementOrOpt(data, s, 2, subseq_matrix);
            break;
        case 5:
            improved = bestImprovementOrOpt(data, s, 3, subseq_matrix);
            break;
        }

        if (improved)
        {
            NL = {1};
        }
        else
        {
            NL.erase(NL.begin() + n);
        }
    }

    // cout << "Diferença entre custo calculado e armazenado (checkCost)(localsearch)): " << s.checkCost(data) << endl;
}