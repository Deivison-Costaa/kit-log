#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <stdexcept>
#include <algorithm>


#include "Solution.hpp"
#include "Data.h"
#include "Metaheuristic.hpp"

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
    if (argc < 2)
    {
        cerr << "Uso: " << argv[0] << " <caminho/para/instancia>" << endl;
        return 1;
    }
    srand(time(NULL));

    string instancePath = argv[1];
    Data data(2, (char *)instancePath.c_str());
    try
    {
        data.read();
        cout << "Instância carregada: " << instancePath << " com " << data.getDimension() << " nós." << endl;
    }
    catch (const exception &e)
    {
        cerr << "Erro ao carregar instância: " << e.what() << endl;
        return 1;
    }
    if (data.getDimension() == 0)
    {
        cerr << "Erro: A dimensão da instância é 0." << endl;
        return 1;
    }

    Metaheuristic meta;

    int n = data.getDimension();
    int max_iter = 10;
    int max_iter_ils = std::min(100, n);

    cout << "\nIniciando GILS-RVND..." << endl;
    cout << "Parâmetros: max_iter = " << max_iter << ", max_iter_ils = " << max_iter_ils << endl;

    timespec start_ts{}, end_ts{};
    clock_gettime(CLOCK_MONOTONIC, &start_ts);

    Solution best_solution;
    try
    {
        best_solution = meta.run(data, max_iter_ils, max_iter);
    }
    catch (const std::exception &e)
    {
        cerr << "Uma exceção ocorreu durante a execução da metaheurística: " << e.what() << endl;
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_ts);
    double time_taken = diffTimespec(start_ts, end_ts);

    cout << "\n--- Resultados Finais do GILS-RVND ---" << endl;
    if (best_solution.sequence.empty())
    {
        cout << "Nenhuma solução válida foi encontrada." << endl;
    }
    else
    {
        cout << "Melhor sequência encontrada: ";
        best_solution.print();
        cout << "Melhor custo MLP encontrado: " << fixed << setprecision(2) << best_solution.cost << endl;
        double cost_difference = best_solution.checkCost(data);
        cout << "Diferença entre custo calculado e armazenado (checkCost): " << fixed << setprecision(2) << cost_difference << endl;
    }
    cout << "Tempo total de execução: " << fixed << setprecision(4) << time_taken << "s" << endl;

    return 0;
}