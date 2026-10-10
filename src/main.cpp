#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include "benchmark.h"

int main()
{
    try
    {
        printSystemInfo();

        const std::vector<LockType> locks =
        {
            LockType::TAS,
            LockType::TTAS,
            LockType::MCS
        };

        // Benchmark all locks
        for (LockType lock_type : locks)
        {
            std::cout
                << "\n========================================\n";

            std::cout
                << "Lock: "
                << lockName(lock_type)
                << "\n";

            std::cout
                << "========================================\n";

            for (int threads : THREAD_COUNTS)
            {
                for (int work : CRITICAL_WORK)
                {
                    std::vector<double> workload_times;
                    std::vector<double> total_times;
                    std::vector<double> throughputs;

                    // Warm-up runs
                    for (int warmup = 0; warmup < NUM_WARMUPS; ++warmup)
                    {
                        runBenchmark(
                            lock_type,
                            threads,
                            ITERATIONS,
                            work,
                            0,
                            -1);
                    }

                    // Measurement runs
                    for (int run = 1; run <= NUM_RUNS; ++run)
                    {
                        BenchmarkResult result =
                            runBenchmark(
                                lock_type,
                                threads,
                                ITERATIONS,
                                work,
                                0,
                                run);

                        workload_times.push_back(result.workload_ms);
                        total_times.push_back(result.total_ms);
                        throughputs.push_back(result.throughput);
                    }

                    // Calculate medians
                    double median_workload = median(workload_times);
                    double median_total = median(total_times);
                    double median_throughput = median(throughputs);

                    // Print results
                    std::cout
                        << std::fixed
                        << std::setprecision(3);

                    std::cout
                        << "threads="
                        << std::setw(2)
                        << threads

                        << " work="
                        << std::setw(4)
                        << work

                        << " workload="
                        << std::setw(10)
                        << median_workload
                        << " ms"

                        << " total="
                        << std::setw(10)
                        << median_total
                        << " ms"

                        << " throughput="
                        << std::setw(12)
                        << median_throughput
                        << " ops/s"

                        << "\n";
                }
            }
        }

        std::cout
            << "\n========================================\n"
            << "Benchmark completed.\n"
            << "========================================\n";
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "ERROR: "
            << e.what()
            << "\n";

        return 1;
    }

    return 0;
}
