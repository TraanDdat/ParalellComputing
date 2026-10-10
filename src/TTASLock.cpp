#include"TTASLock.h"
void TTASLock::lock()
{
        bool expected;
        do
        {
                while(locked.load(std::memory_order_relaxed)){/* Spin */}
                expected = false;
        } while(!locked.compare_exchange_weak(expected, true, std::memory_order_acquire, std::memory_order_relaxed));
}

void TTASLock::unlock()
{
        locked.store(false, std::memory_order_release);
}