#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include <iostream>
#include <chrono>
#include <algorithm>
#include <thread>
class MUTEXLock
{
        public:
                virtual ~MUTEXLock() = default;
                virtual void lock() = 0;
                virtual void unlock() = 0;
};

#endif