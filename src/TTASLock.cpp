#include"TTASLock.h"
void TTASLock::lock()
{
        bool expected;
        // auto backoff = std::chrono::microseconds(1);
        // constexpr auto MAX_BACKOFF = std::chrono::microseconds(8);
        do
        {
                while(locked.load(std::memory_order_relaxed))
                {
                        // std::this_thread::sleep_for(backoff);
                        // backoff = std::min(backoff * 2, MAX_BACKOFF); 
                }
                expected = false;
        } while(!locked.compare_exchange_weak(expected, true, std::memory_order_acquire, std::memory_order_relaxed));
}

void TTASLock::unlock()
{
        locked.store(false, std::memory_order_release);
}