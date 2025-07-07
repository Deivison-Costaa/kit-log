#pragma once

#include <vector>
#include "Solution.hpp"
#include "Data.h"
#include "Subsequence.hpp" // Added for Subsequence type

class LocalSearch
{
private:
    // Methods will now also need access to subseq_matrix and UpdateAllSubseq
    // They will modify s, s.cost, and subseq_matrix upon improvement
    static bool bestImprovementSwap(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix);
    static bool bestImprovement2Opt(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix);
    static bool bestImprovementOrOpt(const Data &data, Solution &s, const int blockSize, std::vector<std::vector<Subsequence>> &subseq_matrix);

public:
    // Run will now take subseq_matrix by reference
    void run(const Data &data, Solution &s, std::vector<std::vector<Subsequence>> &subseq_matrix);
};