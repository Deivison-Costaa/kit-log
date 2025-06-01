#include "Solution.hpp"
#include "Construction.hpp"
#include "LocalSearch.hpp"
#include "Ils.hpp"
#include <random>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

void Ils::run(const Data &data, Solution &bestOfAll, const int maxIter, const int maxIterIls, char constructionMethod)
{
    bestOfAll.cost = INFINITY;
    Construction cons;
    LocalSearch ls;

    for (size_t i = 0; i < maxIter; ++i)
    {
        Solution s;
        cons.runConstruction(data, s, constructionMethod);
        Solution best = s;

        int iterIls = 0;

        while (iterIls <= maxIterIls)
        {
            ls.run(data, s);
            if (s.cost < best.cost)
            {
                best = s;
                iterIls = 0;
            }
            else
            {
                s = perturb(data, best);
                iterIls++;
            }

            if (best.cost < bestOfAll.cost)
            {
                bestOfAll = best;
            }
        }
    }
    return;
}

Solution Ils::perturb(const Data &data, const Solution &best)
{
    Solution newSol = best;
    int n = newSol.sequence.size();

    // Necessitamos de pelo menos alguns pontos para fazer o double bridge
    if (n < 6)
        return newSol; // Retorna a solução original se não for possível fazer perturbação

    // Baseado no exemplo: inicio | segmento1 | meio | segmento2 | fim -> inicio | segmento2 | meio | segmento1 | fim

    std::random_device rd;
    std::mt19937 gen(rd());

    // Tamanho máximo de segmento conforme especificação (|V|/10)
    int maxSegSize = std::max(2, static_cast<int>(n / 10));
    int minSegSize = 2; // Tamanho mínimo de um segmento

    // Escolher aleatoriamente dois segmentos não sobrepostos com tamanhos entre 2 e |V|/10
    std::uniform_int_distribution<> sizeDist(minSegSize, maxSegSize);

    int size1 = sizeDist(gen); // Tamanho do primeiro segmento
    int size2 = sizeDist(gen); // Tamanho do segundo segmento

    // Garantir que haja espaço suficiente para os dois segmentos e as partes entre eles
    int minMidSize = 1; // Tamanho mínimo da seção "meio"

    // Calcular o tamanho total disponível excluindo o início e o fim (que precisam ter pelo menos 1 elemento cada)
    int availableSpace = n - 2;

    // Se não houver espaço suficiente, ajustar os tamanhos dos segmentos
    if (size1 + size2 + minMidSize > availableSpace)
    {
        int excessSize = (size1 + size2 + minMidSize) - availableSpace;

        // Reduzir proporcionalmente os tamanhos dos segmentos
        int reduce1 = excessSize / 2 + excessSize % 2;
        int reduce2 = excessSize / 2;

        size1 = std::max(minSegSize, size1 - reduce1);
        size2 = std::max(minSegSize, size2 - reduce2);

        // Verificar se ainda excede o espaço disponível
        if (size1 + size2 + minMidSize > availableSpace)
        {
            // Simplesmente usar os tamanhos mínimos
            size1 = minSegSize;
            size2 = minSegSize;

            if (size1 + size2 + minMidSize > availableSpace)
            {
                // Não há espaço suficiente mesmo com segmentos mínimos
                return newSol;
            }
        }
    }

    // Vamos definir 5 pontos: início, início do segmento1, início do meio, início do segmento2, início do fim
    std::vector<int> points(5);
    points[0] = 0; // Início

    // Determinar posição do segmento1 (após o início)
    std::uniform_int_distribution<> pos1Dist(1, n - size1 - size2 - minMidSize - 1);
    points[1] = pos1Dist(gen);

    // Determinar onde começa o meio (após o segmento1)
    points[2] = points[1] + size1;

    // Determinar onde começa o segmento2 (após o meio, deixando espaço para o fim)
    int remainingSpace = n - points[2] - size2 - 1;
    int midSize = std::max(minMidSize, remainingSpace / 2); // Garantir tamanho do meio razoável

    std::uniform_int_distribution<> midSizeDist(minMidSize, std::max(minMidSize, remainingSpace - 1));
    midSize = midSizeDist(gen);

    points[3] = points[2] + midSize;

    // Determinar onde começa o fim (após o segmento2)
    points[4] = points[3] + size2;

    // Verificar se todos os pontos são válidos
    if (points[4] >= n)
    {
        // Se ultrapassou o tamanho da sequência, retornar a solução original
        return newSol;
    }

    // Criar a nova sequência no formato: inicio | segmento2 | meio | segmento1 | fim
    std::vector<int> newSequence(n);

    // Copiar o início
    for (int i = points[0]; i < points[1]; ++i)
    {
        newSequence[i] = best.sequence[i];
    }

    // Copiar o segmento2 para depois do início
    for (int i = 0; i < size2; ++i)
    {
        newSequence[points[1] + i] = best.sequence[points[3] + i];
    }

    // Copiar o meio
    for (int i = 0; i < (points[3] - points[2]); ++i)
    {
        newSequence[points[1] + size2 + i] = best.sequence[points[2] + i];
    }

    // Copiar o segmento1 para depois do meio
    for (int i = 0; i < size1; ++i)
    {
        newSequence[points[1] + size2 + (points[3] - points[2]) + i] = best.sequence[points[1] + i];
    }

    // Copiar o fim
    for (int i = points[4]; i < n; ++i)
    {
        newSequence[i] = best.sequence[i];
    }

    newSol.sequence = newSequence;

    // Calcular o novo custo de forma otimizada, apenas recalculando as conexões modificadas
    double newCost = best.cost;

    // Remover custos das conexões antigas
    // inicio-segmento1
    newCost -= data.getDistance(best.sequence[points[1] - 1], best.sequence[points[1]]);

    // segmento1-meio
    newCost -= data.getDistance(best.sequence[points[2] - 1], best.sequence[points[2]]);

    // meio-segmento2
    newCost -= data.getDistance(best.sequence[points[3] - 1], best.sequence[points[3]]);

    // segmento2-fim
    newCost -= data.getDistance(best.sequence[points[4] - 1], best.sequence[points[4]]);

    // Adicionar custos das novas conexões
    // inicio-segmento2
    newCost += data.getDistance(newSequence[points[1] - 1], newSequence[points[1]]);

    // segmento2-meio
    newCost += data.getDistance(newSequence[points[1] + size2 - 1], newSequence[points[1] + size2]);

    // meio-segmento1
    newCost += data.getDistance(newSequence[points[1] + size2 + (points[3] - points[2]) - 1],
                                newSequence[points[1] + size2 + (points[3] - points[2])]);

    // segmento1-fim
    newCost += data.getDistance(newSequence[points[1] + size2 + (points[3] - points[2]) + size1 - 1],
                                newSequence[points[1] + size2 + (points[3] - points[2]) + size1]);

    // Se for um circuito fechado (como no TSP clássico), verificar se o ponto final está ligado ao inicial
    if (best.sequence[0] == best.sequence[n - 1])
    {
        // Custo não muda se o início e o fim permanecem os mesmos
        if (points[0] > 0 || points[4] < n - 1)
        {
            // Remover custo da conexão antiga fim-inicio
            newCost -= data.getDistance(best.sequence[n - 1], best.sequence[0]);

            // Adicionar custo da nova conexão fim-inicio
            newCost += data.getDistance(newSequence[n - 1], newSequence[0]);
        }
    }

    newSol.cost = newCost;

    return newSol;
}