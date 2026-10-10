#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include <string>

struct BenchmarkResult
{
    std::string lock_name;
    int threads;
    int iterations;
    int critical_work;
    int max_backoff;
    int run;
    double workload_ms;
    double total_ms;
    double throughput;
    long long counter;
};

enum class LockType
{
    TAS,
    TTAS,
    MCS
};

// Configuration constants
constexpr int NUM_RUNS = 5;
constexpr int NUM_WARMUPS = 2;
constexpr int ITERATIONS = 1000000;
extern const std::vector<int> THREAD_COUNTS;
extern const std::vector<int> CRITICAL_WORK;

// Functions
extern std::string lockName(LockType type);
extern BenchmarkResult runBenchmark(LockType lock_type, int num_threads, int iterations, int critical_work, int max_backoff, int run);
extern double median(std::vector<double> values);
extern void printSystemInfo();

#endif