#ifndef OPENMPLOCK_H
#define OPENMPLOCK_H
#include <omp.h>
#include "MUTEXLock.h"

class OPENMPLock : public MUTEXLock
{
private:
    omp_lock_t lock_obj;

public:
    OPENMPLock()
    {
        omp_init_lock(&lock_obj);
    }
    ~OPENMPLock() override
    {
        omp_destroy_lock(&lock_obj);
    }
    void lock() override;
    void unlock() override;
    OPENMPLock(const OPENMPLock&) = delete;
    OPENMPLock& operator=(const OPENMPLock&) = delete;
};
#endif