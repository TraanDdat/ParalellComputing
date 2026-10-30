#ifndef PTHREADSPINLOCK_H
#define PTHREADSPINLOCK_H
#include "MUTEXLock.h"
#include <pthread.h>
class PTHREADSPINLock : public MUTEXLock
{
    private:
        pthread_spinlock_t spin;
    public:
        PTHREADSPINLock()
        {
            const int ret = pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);
            if (ret != 0)
            {
                throw std::runtime_error("pthread_spin_init failed");
            }
        }
        ~PTHREADSPINLock() override
        {
            pthread_spin_destroy(&spin);
        }
        PTHREADSPINLock(const PTHREADSPINLock&) = delete;
        PTHREADSPINLock& operator=(const PTHREADSPINLock&) = delete;
        void lock() override;
        void unlock() override;
};
#endif