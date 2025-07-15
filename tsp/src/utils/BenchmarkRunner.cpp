#include "BenchmarkRunner.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <ctime>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <string>

#include "Data.h"
#include "Solution.hpp"
#include "Ils.hpp"

namespace fs = std::filesystem;

std::unordered_map<std::string, std::pair<double, double>>
BenchmarkRunner::readBaseInstances(const std::string &filename)
{
    std::unordered_map<std::string, std::pair<double, double>> baseData;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir baseInstances.txt" << std::endl;
        return baseData;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string name;
        double timeBase, costBase;
        if (!(iss >> name >> timeBase >> costBase))
            continue;
        baseData[name] = {timeBase, costBase};
    }
    return baseData;
}

std::vector<BenchmarkResult>
BenchmarkRunner::runBatch(const std::string &folder, const std::string &baseFile, char method)
{
    auto baseData = readBaseInstances(baseFile);
    std::vector<BenchmarkResult> results;
    const int num_runs_per_instance = 1;

    for (const auto &entry : fs::directory_iterator(folder))
    {
        if (entry.path().extension() == ".tsp")
        {
            std::string instanceName = entry.path().stem().string();
            auto it = baseData.find(instanceName);
            if (it == baseData.end())
            {
                std::cerr << "Aviso: Dados base não encontrados para " << instanceName << std::endl;
                continue;
            }

            Data data(2, (char *)entry.path().c_str());
            data.read();
            Ils ils;

            double totalTimeObtained = 0.0;
            double totalCostObtained = 0.0;
            double totalCostDifference = 0.0;

            int dim = data.getDimension();
            int maxIter = 50;
            int maxIterIls = (dim >= 150) ? dim / 2 : dim;

            for (int k = 0; k < num_runs_per_instance; ++k)
            {
                Solution sol; // Cria uma nova solucao para cada execucao do teste

                timespec start_time{}, end_time{};
                clock_gettime(CLOCK_MONOTONIC, &start_time);
                ils.run(data, sol, maxIter, maxIterIls, method);
                clock_gettime(CLOCK_MONOTONIC, &end_time);

                totalTimeObtained += diffTimespec(start_time, end_time);
                totalCostObtained += sol.cost;
                totalCostDifference += sol.checkCost(data);
            }

            double avgTimeObtained = totalTimeObtained / num_runs_per_instance;
            double avgCostObtained = totalCostObtained / num_runs_per_instance;
            double avgCostDifference = totalCostDifference / num_runs_per_instance;

            double timeBase = it->second.first;
            double costBase = it->second.second;

            results.push_back({instanceName,
                               timeBase,
                               avgTimeObtained,
                               costBase,
                               avgCostObtained,
                               avgCostDifference});
        }
    }
    return results;
}

void BenchmarkRunner::writeResultsToCSV(const std::vector<BenchmarkResult> &results,
                                        const std::string &outputFile)
{
    std::ofstream out(outputFile);
    if (!out.is_open())
    {
        std::cerr << "Erro ao abrir " << outputFile << std::endl;
        return;
    }

    out << "instancia,tempo_base,tempo_obtido,custo_base,custo_obtido,diferenca_custo\n";
    for (const auto &res : results)
    {
        out << res.instanceName << ","
            << res.timeBase << ","
            << res.timeObtained << ","
            << res.costBase << ","
            << res.costObtained << ","
            << res.costDiff << "\n";
    }
    out.close();
}

double BenchmarkRunner::diffTimespec(const timespec &start, const timespec &end)
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