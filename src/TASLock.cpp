#include"TASLock.h"
void TASLock::lock()
{
        // auto backoff = std::chrono::microseconds(1);
        // constexpr auto MAX_BACKOFF = std::chrono::microseconds(8);
        while(flag.test_and_set(std::memory_order_acquire))
        {
                // std::this_thread::sleep_for(backoff);
                // backoff = std::min(backoff * 2, MAX_BACKOFF); 
        }
}
void TASLock::unlock()
{
        flag.clear(std::memory_order_release);
}