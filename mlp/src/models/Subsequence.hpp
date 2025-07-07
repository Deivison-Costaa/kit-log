#pragma once

#include "Solution.hpp"    // Assumindo que Solution está definida aqui
#include "Data.h"        // Assumindo que Data está definida aqui
#include <vector>

struct Subsequence
{
    double T;
    double C;
    int W;
    int first;
    int last;

    inline static Subsequence Concatenate(const Subsequence &sigma_1, const Subsequence &sigma_2, const Data &data)
    {
        Subsequence sigma;
        double temp = data.getDistance(sigma_1.last, sigma_2.first);

        sigma.W = sigma_1.W + sigma_2.W;
        sigma.T = sigma_1.T + temp + sigma_2.T;
        sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;
        sigma.first = sigma_1.first;
        sigma.last = sigma_2.last;
        return sigma;
    }

    Subsequence(int node = 0, bool isDepot = false)
    {
        T = 0.0;
        C = 0.0;
        W = isDepot ? 0 : 1;
        first = node;
        last = node;
    }
};

static void UpdateAllSubseq(const Solution &s, const Data &data,
                     std::vector<std::vector<Subsequence>> &subseq_matrix)
{

    int n_seq_nodes = s.sequence.size(); 
    if (n_seq_nodes == 0)
    {
        subseq_matrix.clear();
        return;
    }

    subseq_matrix.assign(n_seq_nodes, std::vector<Subsequence>(n_seq_nodes));

    for (int i = 0; i < n_seq_nodes; ++i)
    {
        bool is_first_node_of_tour = (i == 0);
        subseq_matrix[i][i] = Subsequence(s.sequence[i], is_first_node_of_tour);
        subseq_matrix[i][i].first = s.sequence[i];
        subseq_matrix[i][i].last = s.sequence[i];
    }

    for (int i = 0; i < n_seq_nodes; ++i)
    {
        for (int j = i + 1; j < n_seq_nodes; ++j)
        {
            subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j - 1], subseq_matrix[j][j], data);
        }
    }

    for (int i = n_seq_nodes - 1; i >= 0; --i)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j + 1], subseq_matrix[j][j], data);
        }
    }
}

static void UpdateAllSubseq(const Solution &s, const Data &data,
                            std::vector<std::vector<Subsequence>> &subseq_matrix, int attI, int attJ)
{
    int n = s.sequence.size();
    if (n == 0)
    {
        subseq_matrix.clear();
        return;
    }

    // Atualizar subsequências de um único nó apenas para attI e attJ
    for (int k : {attI, attJ})
    {
        bool is_first_node_of_tour = (k == 0);
        subseq_matrix[k][k] = Subsequence(s.sequence[k], is_first_node_of_tour);
        subseq_matrix[k][k].first = s.sequence[k];
        subseq_matrix[k][k].last = s.sequence[k];
    }

    // Atualizar subsequências diretas que incluem attI ou attJ
    for (int i = 0; i < n; ++i)
    {
        for (int j = i; j < n; ++j)
        {
            if ((i <= attI && attI <= j) || (i <= attJ && attJ <= j))
            {
                if (i == j)
                {
                    // Já atualizado acima para attI e attJ
                    continue;
                }
                subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j - 1], subseq_matrix[j][j], data);
            }
        }
    }

    //Atualizar subsequências invertidas que incluem attI ou attJ
    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = i; j >= 0; --j)
        {
            if ((j <= attI && attI <= i) || (j <= attJ && attJ <= i))
            {
                if (i == j)
                {
                    // Já atualizado acima para attI e attJ
                    continue;
                }
                subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j + 1], subseq_matrix[j][j], data);
            }
        }
    }
}