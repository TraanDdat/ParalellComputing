#include "mythread.h"

using namespace std;

void test_barrier_throughput(int scale_cnt, int num_barrier);
void test_correctness(int scale_cnt);
void test_scalability(int scale_cnt);
bool check_correctness();

std::vector<std::thread> threads;
atomic_int val_atm_arr[NUM_OF_THREAD] = {0};
int thread_it = 4;
int val = 0;
std::atomic<int> val_atm{0};
std::atomic<bool> testing_perf{false};

int main()
{
    #if NUM_OF_BARRIER > 0
        test_barrier_throughput(NUM_OF_THREAD, NUM_OF_BARRIER);
    #else
        testing_perf.store(false);
        test_correctness(NUM_OF_THREAD);

        testing_perf.store(true);
        test_scalability(NUM_OF_THREAD);
    #endif
    
    if(!check_correctness())
    {
        cout << "[ERROR] Barrier correctness check failed!\n";
    }
    else
    {
        cout << "[INFO] Barrier correctness check passed!\n";
    }

    std::cin.get();
    return 0;
}

void test_correctness(int scale_cnt)
{
    cout << "[INFO] Testing correctness with " << scale_cnt << " (threads)\n";
    cout << "[INFO] [STD_BARRIER=" << STD_BARRIER << "][TEST_PROBLEM=" << TEST_PROB << "][VARIANT_OF_BARRIER=" << VAR << "][NUM_OF_THREAD=" << scale_cnt << "][BACKOFF=" << BACKOFF << "][testing_perf=" << testing_perf << "]\n";
    cout << "Thread_A:  [start] ---- barrier wait --- barrier wait --- [end]\n";
    cout << "Thread_B:     [start] ---- barrier wait --- barrier wait --- [end]\n";
    cout << "Thread_C:         [start] ---- barrier wait --- barrier wait --- [end]\n";
    cout << "..................................................................................\n";

    #if (STD_BARRIER == true)
        std::barrier hreadBar(scale_cnt);
    #else
        mySenseReversingBarrier hreadBar(scale_cnt);
    #endif
    for (int i = 0; i < scale_cnt; i++)
    {
        threads.emplace_back(thread_worker,std::ref(hreadBar),i);
    }
    
    auto start = std::chrono::steady_clock::now();
    for (auto& t : threads)
    {
        t.join();
    }
    threads.clear();
    auto end = std::chrono::steady_clock::now();
    auto time_ex = end - start;
    double seconds = std::chrono::duration<double>(time_ex).count();

    cout << "[INFO] Time execute correctness test = " << seconds*1000000 << " (us)\n";
    cout << "[INFO] END OF TESTING\n";
    cout << "..................................................................................\n";
}

void test_scalability(int scale_cnt)
{
    cout << "[INFO] Testing scalability threads with " << scale_cnt << " (threads)\n";
    cout << "[INFO] [STD_BARRIER=" << STD_BARRIER << "][TEST_PROBLEM=" << TEST_PROB << "][VARIANT_OF_BARRIER=" << VAR << "][NUM_OF_THREAD=" << scale_cnt << "][BACKOFF=" << BACKOFF << "][THREAD_SLEEP=" << THREAD_SLEEP << "][testing_perf=" << testing_perf << "]\n";
    cout << "Thread_[i%3==0]:  [start] ---- barrier wait --- barrier wait --- [end]\n";
    cout << "Thread_[i%3==1]:     [start] ---- barrier wait --- barrier wait --- [end]\n";
    cout << "Thread_[i%3==2]:         [start] ---- barrier wait --- barrier wait --- [end]\n";
    cout << "Thread_[i%3==0]:              [start] ---- barrier wait --- barrier wait --- [end]\n";
    cout << "..................................................................................\n";

    double total_seconds = 0.0;

    // create multiple time testing of scalability
    cout << "[INFO] Execute " << TOTAL_IT << " times testing of " << scale_cnt << " threads join barrier\n";
    for(long long int j = 0; j < TOTAL_IT; j++)
    {
        std::vector<std::thread> threads;
        #if (STD_BARRIER == true)
            std::barrier hreadBar(scale_cnt);
        #else
            mySenseReversingBarrier hreadBar(scale_cnt);
        #endif
        for (int i = 0; i < scale_cnt; i++)
        {
            threads.emplace_back(thread_worker,std::ref(hreadBar),i);
        }

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }
        auto end = std::chrono::steady_clock::now();
        auto time_ex = end - start;
        double seconds = std::chrono::duration<double>(time_ex).count();
        total_seconds += seconds;
    }

    cout << "[INFO] [val_atm = " << val_atm << "]\n";
    cout << "[INFO] [val = " << val << "]\n";
    for (int j = 0; j < scale_cnt; j++)
    {
        cout << "[INFO] [val_atm_arr[" << j << "] = " << val_atm_arr[j].load() << "]\n";
    }
    cout << "[INFO] Time execute " << TOTAL_IT << " times of " << scale_cnt << " threads join barrier = " << total_seconds << " (s)\n";
    cout << "[INFO] Average time execute of " << scale_cnt << " threads join barrier 2 times = " << (total_seconds*1000000/TOTAL_IT) << " (us)\n";
    cout << "[INFO] END OF TESTING\n";
    cout << "..................................................................................\n";
}

void test_barrier_throughput(int scale_cnt, int num_barrier)
{
    double total_seconds = 0.0;

    cout << "[INFO] Testing barrier throughput with " << scale_cnt << " (threads) then execute " << num_barrier << "(barriers)\n";
    cout << "[INFO] [STD_BARRIER=" << STD_BARRIER << "][VARIANT_OF_BARRIER=" << VAR << "][NUM_OF_THREAD=" << scale_cnt << "][NUM_OF_BARRIER="<< num_barrier <<"][BACKOFF=" << BACKOFF << "]\n";  
    cout << "[INFO] Execute " << TOTAL_IT << " times testing of " << scale_cnt << " threads join " << num_barrier << " (barriers)\n";
    cout << "..................................................................................\n";

    for(long long int j = 0; j < TOTAL_IT; j++)
    {
        std::vector<std::thread> threads;
        #if (STD_BARRIER == true)
            std::barrier hreadBar(scale_cnt);
        #else
            mySenseReversingBarrier hreadBar(scale_cnt);
        #endif

        for (int i = 0; i < scale_cnt; i++)
        {
            threads.emplace_back(thread_worker_throughput,std::ref(hreadBar),i,num_barrier);
        }

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }
        auto end = std::chrono::steady_clock::now();
        auto time_ex = end - start;
        double seconds = std::chrono::duration<double>(time_ex).count();
        total_seconds += seconds;
    }

    cout << "[INFO] Time execute " << scale_cnt << " threads join " << num_barrier << " (barrier) = " << total_seconds << " (s)\n";
    cout << "[INFO] Throughout = " << (num_barrier*TOTAL_IT)/total_seconds <<"(barrier/s)\n";
    cout << "[INFO] END OF TESTING\n";
    cout << "..................................................................................\n";
}

bool check_correctness()
{
    cout << "[INFO] Checking correctness of the barrier implementation...\n";
    cout << "..................................................................................\n";

    if (TEST_PROB == 1)
    {
        cout << "[INFO] Test problem 1: Thread i, array val_atm_arr correctness check\n";
        cout << "[INFO] TEST_PROB=" << TEST_PROB << " execute " << TOTAL_IT + 1 << " times" << "\n";
        cout << "[INFO] STD_BARRIER=" << STD_BARRIER << ", VARIANT_OF_BARRIER=" << VAR << "\n";
        cout << "[INFO] NUM_OF_THREAD=" << NUM_OF_THREAD << ", NUM_OF_BARRIER=" << NUM_OF_BARRIER << ", BACKOFF=" << BACKOFF << "\n";
        
        for (long long int i = 0; i < NUM_OF_THREAD; i++)
        {
            if(i%3 != 2)
            {
                if(val_atm_arr[i] != ((1+2)*thread_it)*(1+TOTAL_IT))
                {
                    cout << "[ERROR] Thread " << i << " did not reach the expected barrier count\n";
                    cout << "[ERROR] Expected output of ((1+2)*"<< thread_it << ")*(1+TOTAL_IT) = " << ((1+2)*thread_it)*(1+TOTAL_IT) << "\n";
                    cout << "[ERROR] Thread " << i << " val_atm_arr[i] = " << val_atm_arr[i] << "\n";
                    return false;
                }
            }
        }
        cout << "[PASS] All threads reached the expected barrier count\n";
    }
    else if (TEST_PROB == 2)
    {
        cout << "[INFO] Test problem 2: Shared variable val (non-atomic type) correctness check\n";
        cout << "[INFO] TEST_PROB=" << TEST_PROB << " execute " << TOTAL_IT + 1 << " times" << "\n";
        cout << "[INFO] STD_BARRIER=" << STD_BARRIER << ", VARIANT_OF_BARRIER=" << VAR << "\n";
        cout << "[INFO] NUM_OF_THREAD=" << NUM_OF_THREAD << ", NUM_OF_BARRIER=" << NUM_OF_BARRIER << ", BACKOFF=" << BACKOFF << "\n";

        if(val != ((1+2)*thread_it)*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)))
        {
            cout << "[ERROR] Shared variable val did not reach the expected barrier count\n";
            cout << "[ERROR] Expected output of ((1+2)*"<< thread_it << ")*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)) = " << ((1+2)*thread_it)*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)) << "\n";
            cout << "[ERROR] val = " << val << "\n";
            return false;
        }
        cout << "[PASS] Expected output of ((1+2)*"<< thread_it << ")*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)) = " << ((1+2)*thread_it)*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)) << "\n";
        cout << "[PASS] val = " << val << "\n";
    }
    else if (TEST_PROB == 3)
    {
        cout << "[INFO] Test problem 3: Shared variable val_atm (atomic type) correctness check\n";
        cout << "[INFO] TEST_PROB=" << TEST_PROB << " execute " << TOTAL_IT + 1 << " times" << "\n";
        cout << "[INFO] STD_BARRIER=" << STD_BARRIER << ", VARIANT_OF_BARRIER=" << VAR << "\n";
        cout << "[INFO] NUM_OF_THREAD=" << NUM_OF_THREAD << ", NUM_OF_BARRIER=" << NUM_OF_BARRIER << ", BACKOFF=" << BACKOFF << "\n";

        if(val_atm != ((1+2)*thread_it)*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)))
        {
            cout << "[ERROR] Shared variable val_atm did not reach the expected barrier count\n";
            cout << "[ERROR] Expected output of ((1+2)*"<< thread_it << ")*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)) = " << ((1+2)*thread_it)*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)) << "\n";
            cout << "[ERROR] val_atm = " << val_atm << "\n";
            return false;
        }
        cout << "[PASS] Expected output of ((1+2)*"<< thread_it << ")*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)) = " << ((1+2)*thread_it)*(1+TOTAL_IT)*(NUM_OF_THREAD - (NUM_OF_THREAD/3)) << "\n";
        cout << "[PASS] val_atm = " << val_atm << "\n";
    }
    else if (TEST_PROB == 4)
    {
        cout << "[INFO] Test problem 4: Shared variable val (non-atomic type) correctness check\n";
        cout << "[INFO] TEST_PROB=" << TEST_PROB << " execute " << TOTAL_IT + 1 << " times" << "\n";
        cout << "[INFO] STD_BARRIER=" << STD_BARRIER << ", VARIANT_OF_BARRIER=" << VAR << "\n";
        cout << "[INFO] NUM_OF_THREAD=" << NUM_OF_THREAD << ", NUM_OF_BARRIER=" << NUM_OF_BARRIER << ", BACKOFF=" << BACKOFF << "\n";

        if(val != ((1+2)*thread_it)*(1+TOTAL_IT)*((1+NUM_OF_THREAD)/3))
        {
            cout << "[ERROR] Shared variable val did not reach the expected barrier count\n";
            cout << "[ERROR] Expected output of ((1+2)*"<< thread_it << ")*(1+TOTAL_IT)*((1+NUM_OF_THREAD)/3) = " << ((1+2)*thread_it)*(1+TOTAL_IT)*((1+NUM_OF_THREAD)/3) << "\n";
            cout << "[ERROR] val = " << val << "\n";
            return false;
        }
        cout << "[PASS] Expected output of ((1+2)*"<< thread_it << ")*(1+TOTAL_IT)*((1+NUM_OF_THREAD)/3) = " << ((1+2)*thread_it)*(1+TOTAL_IT)*((1+NUM_OF_THREAD)/3) << "\n";
        cout << "[PASS] val = " << val << "\n";
    }
    return true;
}