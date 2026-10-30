#include<iostream>
#include<stdio.h>
#include<memory>
#include<pthread.h>
#include <condition_variable>
#include <mutex>
#include<thread>
#include<vector>
#include <atomic>
#include <stdexcept>
#include"../include/MUTEXLock.h"
#include"../include/MCSLock.h"
#include"../include/TASLock.h"
#include"../include/TTASLock.h"
#include"../include/PTHREADMUTEXLock.h"
#include"../include/PTHREADSPINLock.h"
#include"../include/OPENMPLock.h"

bool testCounter(int num_threads, int iterations, MUTEXLock *lock);
bool testMutualExclusion(int num_threads, int iterations, MUTEXLock* lock);
bool testRepeatedHandoff(int num_threads, int iterations, MUTEXLock* lock);
int is_prime(int p);
bool thread_entry(int num_threads, int iterations, MUTEXLock* lock);

constexpr int N = 1000003;
constexpr int OUTSIDE_WORK = 100;
constexpr int NUM_THREADS = 8;
constexpr int ITERATION = 1000;
enum class LockType
{
    TAS,
    TTAS,
    MCS,
    PTHREADMUTEX,
    PTHREADSPIN,
    OPENMP
};

std::unique_ptr<MUTEXLock> client_createLock(LockType type)
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
            throw std::runtime_error(
                "Unknown lock type");
    }
}

bool testCounter(int num_threads, int iterations, MUTEXLock *lock)
{
    long long counter = 0;
    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; ++t)
    {
        threads.emplace_back([&,t]()
        {
            for (int i = 0; i < iterations; ++i)
            {
		        lock->lock();
               	++counter;
		        lock->unlock();
            }
        });
    }

    for (auto& thread : threads)    thread.join();

    long long expected = static_cast<long long>(num_threads) * iterations;
    std::cout<<"Expected value: "<< expected << std::endl;
    std::cout<<"Counter value: "<< counter << std::endl;

    return counter == expected;
}

bool testMutualExclusion(int num_threads, int iterations, MUTEXLock* lock)
{
    if (num_threads <= 0 || iterations < 0 || lock == nullptr)
        return false;

    std::atomic<int> activeThreads{0};
    std::atomic<bool> violation{false};
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int t = 0; t < num_threads; ++t)
    {
        threads.emplace_back([&, t]()
        {
            for (int i = 0; i < iterations; ++i)
            {
                lock->lock();
                int previous = activeThreads.fetch_add(1, std::memory_order_relaxed);
                if (previous != 0)
                {
                    violation.store(true, std::memory_order_relaxed);
                }
                activeThreads.fetch_sub(1, std::memory_order_relaxed);
                lock->unlock();
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }
    return !violation.load();
}

bool testRepeatedHandoff(int num_threads, int iterations, MUTEXLock* lock)
{
    if (num_threads <= 0 || iterations < 0 || lock == nullptr)
        return false;

    std::atomic<int> completed{0};
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t)
    {
        threads.emplace_back([&]()
        {
            for (int i = 0; i < iterations; ++i)
            {
                lock->lock();
                lock->unlock();
                completed.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (auto& thread : threads)
        thread.join();

    const int expected = num_threads * iterations;
    const bool passed = completed.load() == expected;

    std::cout << "Expected operations: " << expected << '\n';
    std::cout << "Completed operations: " << completed.load() << '\n';

    return passed;
}

bool thread_entry(int num_threads, int iterations, MUTEXLock* lock)
{
    long long counter = 0;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    auto total_start = std::chrono::steady_clock::now();
    for (int t = 0; t < num_threads; ++t)
    {
        threads.emplace_back([&]()
        {
            for(auto i = 0; i < iterations; ++i)
            {
                lock->lock();
                ++counter;
                bool result = is_prime(N);
                lock->unlock();
                for(auto j = 0; j < OUTSIDE_WORK; ++j)   is_prime(N);
            }
        });
    }
    auto workload_start = std::chrono::steady_clock::now();

    for (auto& thread : threads)    thread.join();

    long long expected = static_cast<long long>(num_threads) * iterations;
    auto workload_end = std::chrono::steady_clock::now();
    double workload_ms = std::chrono::duration<double, std::milli>(workload_end - workload_start).count();
    auto total_end = std::chrono::steady_clock::now();
    double total_ms = std::chrono::duration<double, std::milli>(total_end - total_start).count();
    double throughput = expected / (workload_ms / 1000.0);

    std::cout<<"Expected value: "<< expected << std::endl;
    std::cout<<"Counter value: "<< counter << std::endl;
    std::cout<<"Total Time (ms): "<<total_ms<<std::endl;
    std::cout<<"Workload Time (ms): "<<workload_ms<<std::endl;
    std::cout<<"Throughput (ms): "<<throughput<<std::endl;

    return counter == expected;
}

int is_prime(int p)
{
    int d;
    for(d = 2; d < p; ++d)
    {
        if(p%d == 0) return 0;
    }
    return 1;
}

int main()
{
    const std::vector<LockType> locks =
    {
        LockType::TAS,
        LockType::TTAS,
        LockType::MCS,
        LockType::PTHREADMUTEX,
        LockType::PTHREADSPIN,
        LockType::OPENMP
    };
    for(LockType lock_type : locks)
    {
        std::unique_ptr<MUTEXLock> lock = client_createLock(lock_type);
        bool flag = false;
        if(lock_type == LockType::TAS) std::cout<<"==================== TASLock ===================="<<std::endl;
        else if(lock_type == LockType::TTAS) std::cout<<"==================== TTASLock ===================="<<std::endl;
        else if(lock_type == LockType::MCS) std::cout<<"==================== MCSLock ===================="<<std::endl;
        else if(lock_type == LockType::PTHREADMUTEX) std::cout<<"==================== PTHREADMUTEX ===================="<<std::endl;
        else if(lock_type == LockType::PTHREADSPIN) std::cout<<"==================== PTHREADSPIN ===================="<<std::endl;
        else if(lock_type == LockType::OPENMP) std::cout<<"==================== OPENMP ===================="<<std::endl;
        std::cout<<"========== testCounter =========="<<std::endl;
        flag = testCounter(NUM_THREADS,ITERATION,lock.get());
        if(flag)
        {
            std::cout<<"testCounter is PASSED"<<std::endl;
        }
        else
        {
            std::cout<<"testCounter is FAILED"<<std::endl;
        }
        std::cout<<"========== testMutualExclusion =========="<<std::endl;
        flag = testMutualExclusion(NUM_THREADS,ITERATION,lock.get());
        if(flag)
        {
            std::cout<<"testMutualExclusion is PASSED"<<std::endl;
        }
        else
        {
            std::cout<<"testMutualExclusion is FAILED"<<std::endl;
        }
        std::cout<<"========== testRepeatedHandoff =========="<<std::endl;
        flag = testRepeatedHandoff(NUM_THREADS,ITERATION,lock.get());
        if(flag)
        {
            std::cout<<"testRepeatedHandoff is PASSED"<<std::endl;
        }
        else
        {
            std::cout<<"testRepeatedHandoff is FAILED"<<std::endl;
        }
        std::cout<<"========== test is_prime() =========="<<std::endl;
        flag = thread_entry(NUM_THREADS,ITERATION,lock.get());
        if(flag)
        {
            std::cout<<"is_prime is PASSED"<<std::endl;
        }
        else
        {
            std::cout<<"is_prime is FAILED"<<std::endl;
        }
    }
    return 0;
}