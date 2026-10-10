#include<iostream>
#include<stdio.h>
#include<memory>
#include<pthread.h>
#include<thread>
#include<vector>
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

// class LockTypeClass
// {
//     public:
//         LockTypeClass(){}
//         virtual ~LockTypeClass() = default;
//         virtual std::unique_ptr<MUTEXLock> createLock() = 0;
// };

// class TASLockType : public LockTypeClass
// {
//     public:
//         TASLockType(){}
//         ~TASLockType(){}
//         std::unique_ptr<MUTEXLock> createLock();
// };

// class TTASLockType : public LockTypeClass
// {
//     public:
//         TTASLockType(){}
//         ~TTASLockType(){}
//         std::unique_ptr<MUTEXLock> createLock();
// };

// class MCSLockType : public LockTypeClass
// {
//     public:
//         MCSLockType(){}
//         ~MCSLockType(){}
//         std::unique_ptr<MUTEXLock> createLock();
// };

// std::unique_ptr<MUTEXLock> TASLockType::createLock()
// {
//     return std::make_unique<TASLock>();
// }

// std::unique_ptr<MUTEXLock> TTASLockType::createLock()
// {
//     return std::make_unique<TTASLock>();
// }

// std::unique_ptr<MUTEXLock> MCSLockType::createLock()
// {
//     return std::make_unique<MCSLock>();
// }

std::unique_ptr<MUTEXLock> client_createLock(LockType type)
{
    
    switch (type)
    {
        case LockType::TAS:
            return std::make_unique<TASLock>();

        case LockType::TTAS:
            return std::make_unique<TTASLock>();

        // case LockType::MCS:
        //     return std::make_unique<MCSLock>();

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
        threads.emplace_back([&]()
        {
            for (int i = 0; i < iterations; ++i)
            {
		        lock->lock();
               	++counter;
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

int main()
{
    const std::vector<LockType> locks =
    {
        LockType::TAS,
        LockType::TTAS
        //LockType::MCS
    };
    for(LockType lock_type : locks)
    {
        std::unique_ptr<MUTEXLock> lock = client_createLock(lock_type);
        bool flag = false;
        flag = testCounter(4,100000,lock.get());
        if(flag)
        {
            std::cout<<"PASSED"<<std::endl;
        }
        else
        {
            std::cout<<"FAILED"<<std::endl;
        }
    }
    return 0;
}