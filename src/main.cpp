#include <iostream>
#include <string>
#include <ctime>
#include <filesystem>
#include "Solution.hpp"
#include "Data.h"
#include "Construction.hpp"
#include "LocalSearch.hpp"

using namespace std;
namespace fs = std::filesystem;

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
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " <path/instance>" << endl;
        return 1;
    }

    timespec start{}, end{};

    Data data(argc, argv[1]);
    data.read();

    Solution sol;
    Construction cons;
    LocalSearch lc;
    clock_gettime(CLOCK_MONOTONIC, &start);
    cons.runCI(data, sol);
    lc.run(data, sol);
    clock_gettime(CLOCK_MONOTONIC, &end);

    auto time = diffTimespec(start, end);

    // Exibe a solução
    cout << "Solucao: ";
    sol.print();

    // Exibe o custo armazenado
    cout << "Custo armazenado: " << sol.cost << endl;

    // Verifica se o custo está correto
    double costDifference = sol.checkCost(data);
    cout << "Diferenca de custo (recalculado - armazenado): " << costDifference << endl;

    // Indica se o custo está correto
    if (costDifference == 0)
    {
        cout << "Custo VERIFICADO: Correto" << endl;
    }
    else
    {
        cout << "Custo VERIFICADO: Incorreto (diferenca = " << costDifference << ")" << endl;
    }

    // Exibe o tempo de execução
    cout << "Tempo de execucao: " << time << "s" << endl;

    return 0;
}