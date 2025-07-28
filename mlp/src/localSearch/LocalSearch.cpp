#include "LocalSearch.hpp"
#include <stdlib.h>
#include <algorithm>
#include <cassert>
#include "Subsequence.hpp"
#include <limits>

bool LocalSearch::bestImprovementSwap(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    double bestDelta = 0;
    int bestI = -1, bestJ = -1;
    int N = s.sequence.size() - 2; // Fica mais organizado se definir um N na hora de concatenar
    double sigmaAux;

    //cout << "entrou no swap" << endl;
    for (int i = 1; i <= N; ++i)
    {
        for (int j = i + 1; j <= N; ++j)
        {
            Subsequence sigma = subseq_matrix[0][i - 1];
            sigma = Subsequence::Concatenate(sigma, subseq_matrix[j][j], data);
            if (j >= i + 2)
            {
                sigma = Subsequence::Concatenate(sigma, subseq_matrix[i + 1][j - 1], data); // meio (caso exista)
            }
            sigma = Subsequence::Concatenate(sigma, subseq_matrix[i][i], data);
            sigma = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][N + 1], data);

            // cout << sigma.first << "  " << sigma.last << endl;

            double delta = sigma.C - s.cost;
            if (delta < bestDelta)
            {
                sigmaAux = sigma.C;
                bestDelta = delta;
                bestI = i;
                bestJ = j;
            }
        }
    }

    if (bestDelta < 0)
    {
        double aux = s.cost;
        std::swap(s.sequence[bestI], s.sequence[bestJ]);
        // UpdateAllSubseq(s, data, subseq_matrix, bestI, bestJ);
        UpdateAllSubseq(s, data, subseq_matrix);
        s.cost = subseq_matrix[0][s.sequence.size() - 1].C;
        assert(s.checkCost(data) == 0);
         //cout << "delta: " << bestDelta << " Antes: " << aux << " Depois: " << s.cost << " Sigma: " << sigmaAux << endl;
        return true;
    }
     //cout << "saiu do swap" << endl;
    return false;
}

// 1 for e std::reverse best j + 1
bool LocalSearch::bestImprovement2Opt(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    double bestDelta = 0;
    int bestI = -1, bestJ = -1;
    int N = s.sequence.size() - 2;
    double sigmaAux;
    // cout << "entrou no 2opt" << endl;
    for (int i = 1; i <= N; ++i)
    {
        for (int j = i + 2; j <= N; ++j)
        {
            // Construir a nova sequência: [0,i] + [j,i+1] (invertido) + [j+1,N]
            Subsequence sigma = subseq_matrix[0][i - 1];                                // De 0 até i
            sigma = Subsequence::Concatenate(sigma, subseq_matrix[j][i], data); // De j até i+1 (invertido)
            if (j + 1 <= N + 1)
            {
                sigma = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][N + 1], data);
            }

            //cout << sigma.W << "  " << sigma.T << "  " << sigma.first << "  " << sigma.last << endl;
            
            double delta = sigma.C - s.cost;
            if (delta < bestDelta)
            {
                sigmaAux = sigma.C;
                bestDelta = delta;
                bestI = i;
                bestJ = j + 1;
            }
        }
    }

    if (bestDelta < 0)
    {
        std::reverse(s.sequence.begin() + bestI, s.sequence.begin() + bestJ);
        double aux = s.cost;
        // UpdateAllSubseq(s, data, subseq_matrix, bestI, bestJ);
        UpdateAllSubseq(s, data, subseq_matrix);
        s.cost = subseq_matrix[0][s.sequence.size() - 1].C;
        assert(s.checkCost(data) == 0);
        // cout << "delta: " << bestDelta << " Antes: " << aux << " Depois: " << s.cost << " Sigma: " << sigmaAux << endl;
        return true;
    }
     //cout << "saiu no 2opt" << endl;
    return false;
}

bool LocalSearch::bestImprovementOrOpt(const Data &data, Solution &s, const int blockSize, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    double bestDelta = 0;
    int bestI = -1, bestJ = -1;
    int N = s.sequence.size() - 2;

    for (int i = 1; i <= N - blockSize + 1; ++i)
    {
        for (int j = 0; j < i - 1 || j >= i + blockSize; ++j)
        {
            if (j > N)
                continue; // Garante que j não exceda os limites

            Subsequence sigma;
            Subsequence block = subseq_matrix[i][i + blockSize - 1];

            if (j < i - 1)
            {
                // Mover para TRÁS: nova ordem é [0..j] | bloco | [j+1..i-1] | [i+bs..N]
                sigma = subseq_matrix[0][j];
                sigma = Subsequence::Concatenate(sigma, block, data);
                if (j + 1 <= i - 1)
                {
                    sigma = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][i - 1], data);
                }
                if (i + blockSize <= N + 1)
                {
                    sigma = Subsequence::Concatenate(sigma, subseq_matrix[i + blockSize][N + 1], data);
                }
            }
            else
            { // Mover para FRENTE (j >= i + blockSize)
                // Nova ordem é [0..i-1] | [i+bs..j] | bloco | [j+1..N]
                sigma = subseq_matrix[0][i - 1];
                sigma = Subsequence::Concatenate(sigma, subseq_matrix[i + blockSize][j], data);
                sigma = Subsequence::Concatenate(sigma, block, data);
                if (j + 1 <= N + 1)
                {
                    sigma = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][N + 1], data);
                }
            }

            // cout << sigma.W << "  " << sigma.T << "  " << sigma.first << "  " << sigma.last << endl;

            double delta = sigma.C - s.cost;
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
        double aux = s.cost;
        // A lógica de aplicação da troca está correta
        std::vector<int> block(s.sequence.begin() + bestI, s.sequence.begin() + bestI + blockSize);
        s.sequence.erase(s.sequence.begin() + bestI, s.sequence.begin() + bestI + blockSize);

        // A posição de inserção depende se o movimento foi para frente ou para trás
        int insertPos = (bestJ < bestI) ? bestJ + 1 : bestJ - blockSize + 1;

        s.sequence.insert(s.sequence.begin() + insertPos, block.begin(), block.end());
        UpdateAllSubseq(s, data, subseq_matrix);
        s.cost = subseq_matrix[0][s.sequence.size() - 1].C;
        assert(s.checkCost(data) == 0);
        // cout << "delta: " << bestDelta << " aux: " << aux << " novo: " << s.cost << endl;
        return true;
    }

    return false;
}

void LocalSearch::run(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    UpdateAllSubseq(s, data, subseq_matrix);
    std::vector<int> NL = {1,2,3,4,5};
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
            NL = {1, 2, 3, 4, 5};
        }
        else
        {
            NL.erase(NL.begin() + n);
        }
    }
    // cout << "saiu ls: " << s;
    // cout << "Diferença entre custo calculado e armazenado (checkCost)(localsearch)): " << s.checkCost(data) << endl;
}