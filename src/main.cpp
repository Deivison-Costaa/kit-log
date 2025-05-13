#include <iostream>
#include <string>
#include <ctime>
#include "Solution.hpp"
#include "Data.h"
#include "Construction.hpp"

using namespace std;

double diffTimespec(const timespec &start, const timespec &end)
{
    timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp.tv_sec + (temp.tv_nsec * 1e-9);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " <path/instance>";
        return 1;
    }

    timespec start{}, end{};

    Data data(argc, argv[1]);
    data.read();

    Solution sol;
    Construction cons;
    clock_gettime(CLOCK_MONOTONIC, &start);
    cons.run(data, sol);
    clock_gettime(CLOCK_MONOTONIC, &end);

    auto time = diffTimespec(start, end);
    sol.print();
    cout << sol.cost << endl;
    cout << time << "s" << endl;
}