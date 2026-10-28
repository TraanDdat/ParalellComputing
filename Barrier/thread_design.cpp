#include "mythread.h"

std::mutex mtx;

void threadA_method_1(mySenseReversingBarrier& x, int self)
{
    int i = 0;

    while (i < thread_it)
    {
        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[   ][i=" << i << "][START][A][method_1][val_atm=" << val_atm.load() << "][val=" << val << "][val_atm_arr[" << self << "]=" << val_atm_arr[self].load() << "]\n";
            mtx.unlock();
        }

        #if defined(TEST_PROB)
            #if TEST_PROB == 1
                val_atm_arr[self]++;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 2) || (TEST_PROB == 4)
                val++;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 3)
                val_atm++;
                x.arrive_and_wait(self);
            #else
                #error [ERROR] "TEST_PROB is not implemented"
            #endif
        #else
            x.arrive_and_wait(self);
        #endif

        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[   ][i=" << i << "][END--][A][method_1][val_atm=" << val_atm.load() << "][val=" << val << "][val_atm_arr[" << self << "]=" << val_atm_arr[self].load() << "]\n";
            mtx.unlock();
        }

        i++;
    }
}

void threadA_method_2(mySenseReversingBarrier& x, int self)
{
    for (int i = 0; i < thread_it; i++)
    {
        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[   ][i=" << i << "][START][A][method_2][val_atm=" << val_atm.load() << "][val=" << val << "][val_atm_arr[" << self << "]=" << val_atm_arr[self].load() << "]\n";
            mtx.unlock();
        }

        #if defined(TEST_PROB)
            #if TEST_PROB == 1
                val_atm_arr[self]+=2;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 2) || (TEST_PROB == 4)
                val+=2;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 3)
                val_atm+=2;
                x.arrive_and_wait(self);
            #else
                #error [ERROR] "TEST_PROB is not implemented"
            #endif
        #else
            x.arrive_and_wait(self);
        #endif

        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[   ][i=" << i << "][END--][A][method_2][val_atm=" << val_atm.load() << "][val=" << val << "][val_atm_arr[" << self << "]=" << val_atm_arr[self].load() << "]\n";
            mtx.unlock();
        }
    }
}

void threadB_method_1(mySenseReversingBarrier& x, int self)
{
    int i = 0;

    while (i < thread_it)
    {
        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[   ][i=" << i << "][START][B][method_1][val_atm=" << val_atm.load() << "][val=" << val << "][val_atm_arr[" << self << "]=" << val_atm_arr[self].load() << "]\n";
            mtx.unlock();
        }

        #if defined(TEST_PROB)
            #if TEST_PROB == 1
                val_atm_arr[self]++;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 2)
                val++;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 3)
                val_atm++;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 4)
                // Do nothing for TEST_PROB == 4
                x.arrive_and_wait(self);
            #else
                #error [ERROR] "TEST_PROB is not implemented"
            #endif
        #else
            x.arrive_and_wait(self);
        #endif

        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[   ][i=" << i << "][END--][B][method_1][val_atm=" << val_atm.load() << "][val=" << val << "][val_atm_arr[" << self << "]=" << val_atm_arr[self].load() << "]\n";
            mtx.unlock();
        }

        i++;
    }
}

void threadB_method_2(mySenseReversingBarrier& x, int self)
{
    for (int i = 0; i < thread_it; i++)
    {
        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[   ][i=" << i << "][START][B][method_2][val_atm=" << val_atm.load() << "][val=" << val << "][val_atm_arr[" << self << "]=" << val_atm_arr[self].load() << "]\n";
            mtx.unlock();
        }

        #if defined(TEST_PROB)
            #if TEST_PROB == 1
                val_atm_arr[self]+=2;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 2)
                val+=2;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 3)
                val_atm+=2;
                x.arrive_and_wait(self);
            #elif (TEST_PROB == 4)
                // Do nothing for TEST_PROB == 4
                x.arrive_and_wait(self);
            #else
                #error [ERROR] "TEST_PROB is not implemented"
            #endif
        #else
            x.arrive_and_wait(self);
        #endif

        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[   ][i=" << i << "][END--][B][method_2][val_atm=" << val_atm.load() << "][val=" << val << "][val_atm_arr[" << self << "]=" << val_atm_arr[self].load() << "]\n";
            mtx.unlock();
        }
    }
}

void threadC_method_1(mySenseReversingBarrier& x, int self)
{
    int i = 0;

    while (i < thread_it)
    {
        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[***][i=" << i << "][START][C][method_1][val_atm=" << val_atm.load() << "][val=" << val << "][print]\n";
            for (int j = 0; j < NUM_OF_THREAD; j++)
            {
                cout << "[***][i=" << i << "][START][C][method_1][val_atm_arr[" << j << "]=" << val_atm_arr[j].load() << "][print]\n";
            }
            mtx.unlock();
        }

        x.arrive_and_wait(self);
        
        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[***][i=" << i << "][END--][C][method_1][val_atm=" << val_atm.load() << "][val=" << val << "][print]\n";
            for (int j = 0; j < NUM_OF_THREAD; j++)
            {
                cout << "[***][i=" << i << "][END--][C][method_1][val_atm_arr[" << j << "]=" << val_atm_arr[j].load() << "][print]\n";
            }
            mtx.unlock();
        }

        i++;
    }
}

void threadC_method_2(mySenseReversingBarrier& x, int self)
{
    for (int i = 0; i < thread_it; i++)
    {
        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[***][i=" << i << "][START][C][method_2][method_2][val_atm=" << val_atm.load() << "][val=" << val << "][print]\n";
            for (int j = 0; j < NUM_OF_THREAD; j++)
            {
                cout << "[***][i=" << i << "][START][C][method_2][val_atm_arr[" << j << "]=" << val_atm_arr[j].load() << "][print]\n";
            }
            mtx.unlock();
        }

        x.arrive_and_wait(self);

        if(!testing_perf.load(std::memory_order_relaxed))
        {
            mtx.lock();
            cout << "[***][i=" << i << "][END--][C][method_2][method_1][val_atm=" << val_atm.load() << "][val=" << val << "][print]\n";
            for (int j = 0; j < NUM_OF_THREAD; j++)
            {
                cout << "[***][i=" << i << "][END--][C][method_2][val_atm_arr[" << j << "]=" << val_atm_arr[j].load() << "][print]\n";
            }
            mtx.unlock();
        }
    }
}