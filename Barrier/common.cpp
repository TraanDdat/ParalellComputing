#include "mythread.h"

void threadA(mySenseReversingBarrier& x, int self)
{
    threadA_method_1(x, self);
    threadA_method_2(x, self);
}

void threadB(mySenseReversingBarrier& x, int self)
{
    threadB_method_1(x, self);
    threadB_method_2(x, self);
}

void threadC(mySenseReversingBarrier& x, int self)
{
    threadC_method_1(x, self);
    threadC_method_2(x, self);
}

void threadD(mySenseReversingBarrier& x, int self, long long int num_barriers)
{
    for(long long int i = 0; i < num_barriers; i++)
    {
        #if (THREAD_SLEEP == 0)
            /* Do nothing */
        #else
            for (int j = 0; j < THREAD_SLEEP; ++j) {
                // Busy-wait
            }
        #endif
        x.arrive_and_wait(self);
    }
}

void thread_worker(mySenseReversingBarrier& x, int i)
{
    switch (i%3)
    {
    case 0:
        threadA(x, i);
        break;
    case 1:
        threadB(x, i);
        break;
    case 2:
        threadC(x, i);
        break;
    default:
        break;
    }
}

void thread_worker_throughput(mySenseReversingBarrier& x, int i, long long int num_barriers)
{
    threadD(x, i, num_barriers);
}