#include <iostream>
#include <string>
#include <ctime>
#include <filesystem>
#include "Solution.hpp"
#include "Data.h"
#include "Ils.hpp"
#include "BenchmarkRunner.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    char method = 'c'; // Padrão: Cheapest Insertion
    string instancePath;
    bool runAll = false;

    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if (arg == "-n")
        {
            method = 'n';
        }
        else if (arg == "-c")
        {
            method = 'c';
        }
        else if (arg == "--all")
        {
            runAll = true;
        }
        else if (arg[0] != '-')
        {
            instancePath = arg;
        }
        else
        {
            cerr << "Uso: " << argv[0] << " [-n | -c] [--all] [path/instance]" << endl;
            return 1;
        }
    }

    if (runAll)
    {
        string baseFile = "baseInstances.txt";
        string folder = "instances/";
        string outputFile = "results_3.csv";

        auto results = BenchmarkRunner::runBatch(folder, baseFile, method);
        BenchmarkRunner::writeResultsToCSV(results, outputFile);
        cout << "Resultados salvos em " << outputFile << endl;
    }
    else
    {
        if (instancePath.empty())
        {
            cerr << "Uso: " << argv[0] << " [-n | -c] <path/instance>" << endl;
            return 1;
        }

        Data data(2, (char*)instancePath.c_str());
        data.read();

        Solution sol;
        Ils ils;
        int maxIter = 50;
        int dim = data.getDimension();
        int maxIterIls = (dim >= 150) ? dim / 2 : dim;

        timespec start{}, end{};
        clock_gettime(CLOCK_MONOTONIC, &start);
        ils.run(data, sol, maxIter, maxIterIls, method);
        clock_gettime(CLOCK_MONOTONIC, &end);

        double time = BenchmarkRunner::diffTimespec(start, end);

        cout << "Solucao: ";
        sol.print();
        cout << "Custo armazenado: " << sol.cost << endl;

        double costDifference = sol.checkCost(data);
        cout << "Diferenca de custo (recalculado - armazenado): " << costDifference << endl;
        cout << "Custo VERIFICADO: " << (costDifference == 0 ? "Correto" : "Incorreto") << endl;
        cout << "Tempo de execucao: " << time << "s" << endl;
    }

    return 0;
}