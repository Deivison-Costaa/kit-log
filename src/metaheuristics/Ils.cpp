#include "Solution.hpp"
#include "Construction.hpp"
#include "LocalSearch.hpp"
#include "Ils.hpp"

void Ils::run(const Data &data, Solution &bestOfAll, const int maxIter, const int maxIterIls, char constructionMethod)
{
    bestOfAll.cost = INFINITY;
    Construction cons;
    LocalSearch ls;

    for (size_t i = 0; i < maxIter; ++i)
    {
        Solution s;
        cons.runConstruction(data, s, constructionMethod);
        
    }
    
}


Solution Ils::perturb(const Solution &best)
{

}