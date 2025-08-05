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
        s = cons.run(data);
        Solution best = s;  
        // best.print();
        // getchar();
        ////cout << "bestofall " << bestOfAll.cost << endl;

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
    const int min_size = 2;
    const int max_size = std::ceil((n - 1) / 10);

    if (max_size < min_size) return s_pert; //instance < 10
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> len_dist(min_size, max_size);

    int len1 = len_dist(gen);
    int len2 = len_dist(gen);
    int max_a = n - len1 - len2 - 2;

    if (max_a < 1) return best;

    std::uniform_int_distribution<int> a_dist(1, max_a);
    int a = a_dist(gen);
    int b = a + len1;
    int max_c = n - len2 - 1;
    
    std::uniform_int_distribution<int> c_dist(b + 1, max_c);
    int c = c_dist(gen);
    int d = c + len2;
    double delta =  - data.getDistance(best.sequence[a - 1], best.sequence[a]) 
                    - data.getDistance(best.sequence[b - 1], best.sequence[b]) 
                    - data.getDistance(best.sequence[c - 1], best.sequence[c]) 
                    - data.getDistance(best.sequence[d - 1], best.sequence[d]) 
                    + data.getDistance(best.sequence[a - 1], best.sequence[c]) 
                    + data.getDistance(best.sequence[d - 1], best.sequence[b]) 
                    + data.getDistance(best.sequence[c - 1], best.sequence[a]) 
                    + data.getDistance(best.sequence[b - 1], best.sequence[d]);

    s_pert.cost = best.cost + delta;
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