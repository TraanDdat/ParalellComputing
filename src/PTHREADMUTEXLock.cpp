#include "PTHREADMUTEXLock.h"

void PTHREADMUTEXLock::lock()
{
    pthread_mutex_lock(&mutex);
}

void PTHREADMUTEXLock::unlock()
{
    pthread_mutex_unlock(&mutex);
}