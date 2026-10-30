#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <pthread.h>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "benchmark.h"
#include "TASLock.h"
#include "TTASLock.h"
#include "MCSLock.h"
#include "MUTEXLock.h"
#include "PTHREADMUTEXLock.h"
#include "PTHREADSPINLock.h"
#include "OPENMPLock.h"
#include "PerformanceMetrics.h"

const std::vector<int> THREAD_COUNTS =
{
    1,
    2,
    4,
    8,
    16,
    24
};

const std::vector<int> CRITICAL_WORK =
{
    1,
    10,
    100
};

std::string lockName(LockType type)
{
    switch (type)
    {
        case LockType::TAS:
            return "TAS";

        case LockType::TTAS:
            return "TTAS";

        case LockType::MCS:
            return "MCS";

        case LockType::PTHREADMUTEX:
            return "PTHREADMUTEX";

        case LockType::PTHREADSPIN:
            return "PTHREADSPIN";
    
        case LockType::OPENMP:
            return "OPENMP";

        default:
            return "Unknown";
    }
}

std::unique_ptr<MUTEXLock> createLock(
    LockType type,
    int max_backoff)
{
    switch (type)
    {
        case LockType::TAS:
            return std::make_unique<TASLock>();

        case LockType::TTAS:
            return std::make_unique<TTASLock>();

        case LockType::MCS:
            return std::make_unique<MCSLock>();

        case LockType::PTHREADMUTEX:
            return std::make_unique<PTHREADMUTEXLock>();

        case LockType::PTHREADSPIN:
            return std::make_unique<PTHREADSPINLock>();

        case LockType::OPENMP:
            return std::make_unique<PTHREADSPINLock>();

        default:
            throw std::runtime_error("Unknown lock type");
    }
}

class StartBarrier
{
private:
    std::mutex mutex;
    std::condition_variable cv;
    int count;
    int arrived = 0;
    bool start = false;
public:
    explicit StartBarrier(int count_): count(count_){}

    void wait()
    {
        std::unique_lock<std::mutex> lock(mutex);
        ++arrived;
        cv.wait(lock,[&]()
            {
                return start;
            });
    }

    void release()
    {
        std::lock_guard<std::mutex> lock(mutex);

        start = true;

        cv.notify_all();
    }

    bool allReady()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return arrived == count;
    }
};

BenchmarkResult runBenchmark(\
    LockType lock_type,\
    int num_threads,\
    int iterations,\
    int critical_work,\
    int max_backoff,\
    int run)
{
    FILETIME start_kernel{}, start_user{};

    if (!getCpuTime(start_kernel, start_user))
    {
        std::cerr << "Failed to get CPU time\n";
    }

    auto start = std::chrono::steady_clock::now();
    auto lock = createLock(lock_type,max_backoff);

    long long shared_counter = 0;

    StartBarrier start_barrier(num_threads);

    std::vector<std::thread> threads;

    threads.reserve(num_threads);

    auto total_start = std::chrono::steady_clock::now();

    for (int t = 0; t < num_threads; ++t)
    {
        threads.emplace_back(
            [&]()
            {
                // Wait until all threads are ready
                start_barrier.wait();
                for (int i = 0; i < iterations; ++i)
                {
                    lock->lock();
                    // Critical section
                    for (int j = 0; j < critical_work; ++j)
                    {
                        ++shared_counter;
                    }
                    lock->unlock();
                }
            });
    }
    {
        while (!start_barrier.allReady())
        std::this_thread::yield();
    }

    auto workload_start = std::chrono::steady_clock::now();

    // Release all threads
    start_barrier.release();

    for (auto& thread : threads)
    {
        thread.join();
    }
    auto end = std::chrono::steady_clock::now();

    double wall_time_ms =
        std::chrono::duration<double, std::milli>(
            end - start
        ).count();

    PerformanceMetrics metrics{};

    if (collectMetrics(wall_time_ms,start_kernel,start_user,metrics))
    {
        printMetrics(metrics);
    }
    auto workload_end = std::chrono::steady_clock::now();

    auto total_end = std::chrono::steady_clock::now();

    double workload_ms = std::chrono::duration<double, std::milli>(workload_end - workload_start).count();

    double total_ms = std::chrono::duration<double, std::milli>(total_end - total_start).count();

    double operations = static_cast<double>(num_threads) * static_cast<double>(iterations);

    double throughput = operations / (workload_ms / 1000.0);

    long long expected = static_cast<long long>(num_threads) * static_cast<long long>(iterations) * static_cast<long long>(critical_work);

    if (shared_counter != expected)
    {
        throw std::runtime_error(
            "ERROR: counter mismatch for " +
            lockName(lock_type) +
            " expected=" +
            std::to_string(expected) +
            " actual=" +
            std::to_string(shared_counter));
    }

    return
    {
        lockName(lock_type),
        num_threads,
        iterations,
        critical_work,
        max_backoff,
        run,
        workload_ms,
        total_ms,
        throughput,
        shared_counter
    };
}

// ============================================================
// Median
// ============================================================

double median(std::vector<double> values)
{
    std::sort(values.begin(),values.end());

    const size_t n = values.size();

    if (n % 2 == 0)
    {
        return (values[n / 2 - 1] + values[n / 2]) / 2.0;
    }

    return values[n / 2];
}

void printSystemInfo()
{
    std::cout
        << "========================================\n"
        << " Mutex Lock Benchmark\n"
        << "========================================\n";

    std::cout
        << "Hardware concurrency: "
        << std::thread::hardware_concurrency()
        << "\n";
    std::cout<< "Iterations: "<< ITERATIONS<< "\n";
    std::cout<< "Runs: "<< NUM_RUNS<< "\n";
    std::cout<< "Warmups: "<< NUM_WARMUPS<< "\n";
    std::cout
        << "========================================\n\n";
}