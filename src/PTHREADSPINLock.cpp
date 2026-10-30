#include "PTHREADSPINLock.h"
void PTHREADSPINLock::lock()
{
    const int ret = pthread_spin_lock(&spin);
    if (ret != 0)
    {
        throw std::runtime_error("pthread_spin_lock failed");
    }
}
void PTHREADSPINLock::unlock()
{
    const int ret = pthread_spin_unlock(&spin);
    if (ret != 0)
    {
        throw std::runtime_error("pthread_spin_unlock failed");
    }
}