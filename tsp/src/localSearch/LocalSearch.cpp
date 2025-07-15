#include "LocalSearch.hpp"
#include <stdlib.h>
#include <algorithm>

bool LocalSearch::bestImprovementSwap(const Data &data, Solution &s)
{
    double bestDelta = 0.0;
    int bestI, bestJ;

    for(int i = 1; i < s.sequence.size() - 1; ++i)
    {
        int vi = s.sequence[i];
        int viPrev = s.sequence[i - 1];
        int viNext = s.sequence[i + 1];

        for(int j = i + 1; j < s.sequence.size() - 1; ++j)
        {
            int vj = s.sequence[j];
            int vjPrev = s.sequence[j - 1];
            int vjNext = s.sequence[j + 1];

            double delta = 0;

            if (j == i + 1 || j == i + 2)
            {
                delta = - data.getDistance(viPrev, vi) - data.getDistance(vj, vjNext)
                        + data.getDistance(viPrev, vj) + data.getDistance(vi, vjNext);
            }else
            {
                delta = - data.getDistance(viPrev, vi) - data.getDistance(vi, viNext) + data.getDistance(viPrev, vj) 
                        + data.getDistance(vj, viNext) - data.getDistance(vjPrev, vj) - data.getDistance(vj, vjNext) 
                        + data.getDistance(vjPrev, vi) + data.getDistance(vi, vjNext);
            }
            
            if (delta < bestDelta)
            {
                bestDelta = delta;
                bestI = i;
                bestJ = j;
            }
            
        }
    }

    if(bestDelta < 0)
    {
        std::swap(s.sequence[bestI], s.sequence[bestJ]);
        s.cost += bestDelta;
        return true;
    }
    return false;
}


bool LocalSearch::bestImprovement2Opt(const Data &data, Solution &s)
{
    double bestDelta = 0.0;
    int reverseI, reverseJ;

    for(int i = 0; i < s.sequence.size() - 1; ++i)
    {
        int vi = s.sequence[i];
        int viNext = s.sequence[i + 1];

        for(int j = i + 2; j < s.sequence.size() - 1; ++j)
        {
            int vj = s.sequence[j];
            int vjNext = s.sequence[j + 1];

            double delta =  - data.getDistance(vi, viNext) + data.getDistance(vi, vj)
                            - data.getDistance(vj, vjNext) + data.getDistance(viNext, vjNext);
            
            if(delta < bestDelta)
            {
                bestDelta = delta;
                reverseI = i + 1;
                reverseJ = j + 1; //lembre-se: preste atenção na documentação [a, b)
            }
        }
    }
    if(bestDelta < 0)
    {
        std::reverse(s.sequence.begin() + reverseI, s.sequence.begin() + reverseJ);
        s.cost += bestDelta;
        return true;
    }
    return false;
}

bool LocalSearch::bestImprovementOrOpt(const Data &data, Solution &s, const int blockSize)
{
    double bestDelta = 0.0;
    int bestI, bestJ;

    for(int i = 1; i < s.sequence.size() - blockSize - 1; ++i)
    {
        int viPrev = s.sequence[i - 1];
        int viFirst = s.sequence[i];
        int viLast = s.sequence[i + blockSize - 1];
        int viNext = s.sequence[i + blockSize];

        double deltaVi = - data.getDistance(viPrev, viFirst) - data.getDistance(viLast, viNext) + data.getDistance(viPrev, viNext);

        for(int j = 0; j < s.sequence.size() - 1; ++j)
        {
            if (j >= i - 1 && j <= i + blockSize - 1) continue;
            if (j == i - 1 && blockSize == 1) continue;

            int vj = s.sequence[j];
            int vjNext = s.sequence[j + 1];

            double delta =  deltaVi - data.getDistance(vj, vjNext) + data.getDistance(vj, viFirst) + data.getDistance(viLast, vjNext);

            if (delta < bestDelta)
            {
                bestDelta = delta;
                bestI = i;
                bestJ = j;
            }
        }
    }
    if(bestDelta < 0)
    {
        std::vector<int> block(s.sequence.begin() + bestI, s.sequence.begin() + bestI + blockSize);
        s.sequence.erase(s.sequence.begin() + bestI, s.sequence.begin() + bestI + blockSize);
        int insertPos = (bestJ >= bestI) ? bestJ - blockSize + 1 : bestJ + 1;  //precisa ajustar por conta da remoção anterior
        s.sequence.insert(s.sequence.begin() + insertPos, block.begin(), block.end());
        s.cost += bestDelta;
        return true;
    }
    return false;
}

void LocalSearch::run(const Data &data, Solution &s)
{
    std::vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;

    while (!NL.empty())
    {
        int n = rand() % NL.size();

        switch (NL[n])
        {
        case 1:
            improved = bestImprovementSwap(data, s);
            break;
        case 2:
            improved = bestImprovement2Opt(data, s);
            break;
        case 3:
            improved = bestImprovementOrOpt(data, s, 1);
            break;
        case 4:
            improved = bestImprovementOrOpt(data, s, 2);
            break;
        case 5:
            improved = bestImprovementOrOpt(data, s, 3);
            break;
        }

        if(improved)
        {
            NL = {1, 2, 3, 4, 5};
        }else{
            NL.erase(NL.begin() + n);
        }
    }
    
}