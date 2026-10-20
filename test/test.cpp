#include<iostream>
#include<stdio.h>
#include<memory>
#include<pthread.h>
#include<thread>
#include<vector>
#include <atomic>
#include <stdexcept>
#include"../include/MUTEXLock.h"
#include"../include/MCSLock.h"
#include"../include/TASLock.h"
#include"../include/TTASLock.h"

enum class LockType
{
    TAS,
    TTAS,
    MCS
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
        default:
            throw std::runtime_error(
                "Unknown lock type");
    }
}

bool testCounter(
    int num_threads,
    int iterations,
    MUTEXLock *lock)
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
                std::cout <<"Thread Order: "<< t <<"|"<<"Thread ID: "<<std::this_thread::get_id()<<"|"<<"Counter: "<<counter<<std::endl;
		        lock->unlock();
            }
        });
    }

    for (auto& thread : threads)
        thread.join();

    long long expected = static_cast<long long>(num_threads) * iterations;
    std::cout<<"Expected value: "<< expected << std::endl;
    std::cout<<"Counter value: "<< counter << std::endl;

    return counter == expected;
}

bool testMutualExclusion(
    int num_threads,
    int iterations,
    MUTEXLock* lock)
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

                // Ghi nhận số thread đang trong critical section.
                int previous = activeThreads.fetch_add(
                    1, std::memory_order_relaxed);

                if (previous != 0)
                {
                    violation.store(
                        true, std::memory_order_relaxed);
                }

                activeThreads.fetch_sub(
                    1, std::memory_order_relaxed);

                lock->unlock();
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << "Mutual exclusion: "
              << (violation.load() ? "FAIL" : "PASS")
              << '\n';

    return !violation.load();
}

int main()
{
    const std::vector<LockType> locks =
    {
        LockType::TAS,
        LockType::TTAS,
        LockType::MCS
    };
    for(LockType lock_type : locks)
    {
        std::unique_ptr<MUTEXLock> lock = client_createLock(lock_type);
        bool flag = false;
        std::cout<<"testCounter"<<std::endl;
        flag = testCounter(4,10,lock.get());
        if(flag)
        {
            std::cout<<"testCounter is PASSED"<<std::endl;
        }
        else
        {
            std::cout<<"testCounter is FAILED"<<std::endl;
        }
        std::cout<<"testMutualExclusion"<<std::endl;
        flag = testMutualExclusion(4,10,lock.get());
        if(flag)
        {
            std::cout<<"testMutualExclusion is PASSED"<<std::endl;
        }
        else
        {
            std::cout<<"testMutualExclusion is FAILED"<<std::endl;
        }
    }
    return 0;
}