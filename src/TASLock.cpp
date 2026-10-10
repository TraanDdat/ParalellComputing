#include"TASLock.h"
void TASLock::lock()
{
        while(flag.test_and_set(std::memory_order_acquire));
}
void TASLock::unlock()
{
        flag.clear(std::memory_order_release);
}