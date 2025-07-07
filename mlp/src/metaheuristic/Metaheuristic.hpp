#pragma once

#include "Data.h"
#include "Solution.hpp"
#include "Subsequence.hpp"
#include <iostream>

/**
 * @class Metaheuristic
 * @brief Implementa o algoritmo GILS-RVND para o Problema do Caixeiro Viajante.
 *
 * Esta classe orquestra as fases de construção, busca local e perturbação
 * para encontrar soluções de alta qualidade.
 */
class Metaheuristic
{
public:
    /**
     * @brief Executa o algoritmo GILS-RVND.
     *
     * @param data Os dados do problema (matriz de distâncias).
     * @param max_iter_ils Número máximo de iterações do ILS sem melhoria.
     * @param max_iter O número total de vezes que o ILS será executado a partir de uma nova solução inicial.
     * @return A melhor solução encontrada.
     */
    Solution run(const Data &data, int max_iter_ils, int max_iter);

private:
    /**
     * @brief Aplica a perturbação Double-Bridge na solução.
     *
     * Este movimento remove 4 arestas e reconecta o tour de forma a
     * escapar de ótimos locais, trocando dois segmentos não adjacentes de lugar.
     *
     * @param s A solução a ser perturbada.
     * @param data Os dados do problema.
     * @param subseq_matrix A matriz de subsequências, que será usada para recalcular o custo.
     */
    Solution perturb(const Data &data, const Solution &best, std::vector<std::vector<Subsequence>> &subseq_matrix);
};