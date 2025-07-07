#pragma once

#include <vector>
#include <iostream>
#include "Data.h"

struct Solution
{
    std::vector<int> sequence;
    double cost;

    void print() const;
    int checkCost(const Data &data) const;
};

// Sobrecarga do operador <<
std::ostream &operator<<(std::ostream &os, const Solution &sol);
