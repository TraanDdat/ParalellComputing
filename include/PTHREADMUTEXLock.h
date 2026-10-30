#ifndef PTHREADMUTEXLOCK_H
#define PTHREADMUTEXLOCK_H
#include "MUTEXLock.h"
#include <pthread.h>
class PTHREADMUTEXLock : public MUTEXLock
{
    private:
        pthread_mutex_t mutex;
    public:
        PTHREADMUTEXLock()
        {
            pthread_mutex_init(&mutex, nullptr);
        }
        ~PTHREADMUTEXLock() override
        {
            pthread_mutex_destroy(&mutex);
        }
        PTHREADMUTEXLock(const PTHREADMUTEXLock&) = delete;
        PTHREADMUTEXLock& operator=(const PTHREADMUTEXLock&) = delete;
        void lock() override;
        void unlock() override;
};
#endif