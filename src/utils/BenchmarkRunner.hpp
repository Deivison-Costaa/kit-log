#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct BenchmarkResult
{
    std::string instanceName;
    double timeBase;
    double timeObtained;
    double costBase;
    double costObtained;
    double costDiff;
};

class BenchmarkRunner
{
public:
    static std::unordered_map<std::string, std::pair<double, double>>
    readBaseInstances(const std::string &filename);

    static std::vector<BenchmarkResult>
    runBatch(const std::string &folder, const std::string &baseFile, char method);

    static void writeResultsToCSV(const std::vector<BenchmarkResult> &results,
                                  const std::string &outputFile);

    static double diffTimespec(const timespec &start, const timespec &end);
};
