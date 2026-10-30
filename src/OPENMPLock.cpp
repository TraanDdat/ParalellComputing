#include "OPENMPLock.h"

void OPENMPLock::lock()
{
    omp_set_lock(&lock_obj);
}
void OPENMPLock::unlock()
{
    omp_unset_lock(&lock_obj);
}