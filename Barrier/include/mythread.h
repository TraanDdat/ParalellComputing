#include "common.h"
#include "myBarrier.h"
#include <barrier>

#ifndef NUM_OF_THREAD
#define NUM_OF_THREAD 3
#endif

#if (STD_BARRIER == true)
    extern void threadA(std::barrier<>& x, int self);
    extern void threadB(std::barrier<>& x, int self);
    extern void threadC(std::barrier<>& x, int self);
    extern void threadD(std::barrier<>& x, int self);
    extern void thread_worker(std::barrier<>& x, int self);
    extern void thread_worker_throughput(std::barrier<>& x, int i, long long int num_barriers);
    extern void threadA_method_1(std::barrier<>& x, int self);
    extern void threadA_method_2(std::barrier<>& x, int self);
    extern void threadB_method_1(std::barrier<>& x, int self);
    extern void threadB_method_2(std::barrier<>& x, int self);
    extern void threadC_method_1(std::barrier<>& x, int self);
    extern void threadC_method_2(std::barrier<>& x, int self);
#else
    extern void threadA(mySenseReversingBarrier& x, int self);
    extern void threadB(mySenseReversingBarrier& x, int self);
    extern void threadC(mySenseReversingBarrier& x, int self);
    extern void threadD(mySenseReversingBarrier& x, int self);
    extern void thread_worker(mySenseReversingBarrier& x, int self);
    extern void thread_worker_throughput(mySenseReversingBarrier& x, int i, long long int num_barriers);
    extern void threadA_method_1(mySenseReversingBarrier& x, int self);
    extern void threadA_method_2(mySenseReversingBarrier& x, int self);
    extern void threadB_method_1(mySenseReversingBarrier& x, int self);
    extern void threadB_method_2(mySenseReversingBarrier& x, int self);
    extern void threadC_method_1(mySenseReversingBarrier& x, int self);
    extern void threadC_method_2(mySenseReversingBarrier& x, int self);
#endif

extern int thread_it;
extern int val;
extern std::atomic<int> val_atm;
extern atomic_int val_atm_arr[NUM_OF_THREAD];
extern std::vector<std::thread> threads;