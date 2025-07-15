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
        }
        if (best.cost < bestOfAll.cost)
        {
            bestOfAll = best;
        }
    }
    return;
}

Solution Ils::perturb(const Data &data, const Solution &best)
{
    Solution s_pert = best;      
    int n = s_pert.sequence.size() - 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> indices;
    for (int i = 1; i < n; ++i)
    {
        indices.push_back(i); //isso foi desespero pq aleatoriedade tava dando ruim, alterar dps (mas não muda muito(agora que pensei melhor eles usam modulo,
        //então muda sim)) 
    }
    std::shuffle(indices.begin(), indices.end(), gen);
    int a = indices[0];
    int b = indices[1];
    int c = indices[2];
    int d = indices[3];

    std::vector<int> selected = {a, b, c, d};
    std::sort(selected.begin(), selected.end());
    a = selected[0];
    b = selected[1];
    c = selected[2];
    d = selected[3];

    // Calcula o delta
    double delta = -data.getDistance(best.sequence[a - 1], best.sequence[a]) - data.getDistance(best.sequence[b - 1], best.sequence[b])
                   - data.getDistance(best.sequence[c - 1], best.sequence[c]) - data.getDistance(best.sequence[d - 1], best.sequence[d])
                   + data.getDistance(best.sequence[a - 1], best.sequence[c]) + data.getDistance(best.sequence[d - 1], best.sequence[b]) 
                   + data.getDistance(best.sequence[c - 1], best.sequence[a]) + data.getDistance(best.sequence[b - 1], best.sequence[d]);

    // Atualiza o custo
    s_pert.cost = best.cost + delta;

    //possivelmente dá pra melhorar isso (novo vector)
    std::vector<int> new_seq;
    new_seq.reserve(s_pert.sequence.size());
    new_seq.insert(new_seq.end(), s_pert.sequence.begin(), s_pert.sequence.begin() + a);
    new_seq.insert(new_seq.end(), s_pert.sequence.begin() + c, s_pert.sequence.begin() + d);
    new_seq.insert(new_seq.end(), s_pert.sequence.begin() + b, s_pert.sequence.begin() + c);
    new_seq.insert(new_seq.end(), s_pert.sequence.begin() + a, s_pert.sequence.begin() + b);
    new_seq.insert(new_seq.end(), s_pert.sequence.begin() + d, s_pert.sequence.end());
    s_pert.sequence = new_seq;

    return s_pert;
}