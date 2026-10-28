#include "common.h"

class mySenseReversingBarrier
{
    private:
        int n_thread;

        #if defined(VAR)
            #if (VAR == 1) || (VAR == 2) || (VAR == 3) || (VAR == 4)
                std::atomic<int> cnt = 0;
            #elif VAR == 5
                std::atomic<unsigned> cnt = 0;
            #elif VAR == 6
                std::atomic<uint64_t> cnt = 0;   
            #else
                #error [ERROR] "VAR is not implemented"
            #endif
        #endif
        
        std::atomic<bool> sense{true};
        // std::vector<int> lsense;

    public:
        mySenseReversingBarrier()
        {
            n_thread = 8;
        }

        mySenseReversingBarrier(int n_input)
        {
            n_thread = n_input;
        }
        
        #if defined(VAR)
            #if (VAR == 1)
                void arrive_and_wait(int in_self)
                {
                    // toggle itself
                    static thread_local bool lsense = true;
                    lsense = !lsense;
                    if(cnt.fetch_add(1, std::memory_order_seq_cst) == (n_thread - 1))
                    {
                        cnt.store(0, std::memory_order_seq_cst);
                        sense.store(lsense, std::memory_order_seq_cst);
                    }
                    else
                    {
                        while (sense.load(std::memory_order_seq_cst) != lsense)
                        {
                            /* spin */
                            #if (BACKOFF > 0)
                                for (int i = 0; i < BACKOFF; ++i) {
                                    // Busy-wait
                                }
                            #endif
                        }
                    }
                }
            #elif (VAR == 2)
                void arrive_and_wait(int in_self)
                {
                    // toggle itself
                    static thread_local bool lsense = true;
                    lsense = !lsense;
                    if(cnt.fetch_add(1, std::memory_order_acq_rel) == (n_thread - 1))
                    {
                        cnt.store(0, std::memory_order_relaxed);
                        sense.store(lsense, std::memory_order_release);
                    }
                    else
                    {
                        while (sense.load(std::memory_order_acquire) != lsense)
                        {
                            /* spin */
                            #if (BACKOFF > 0)
                                for (int i = 0; i < BACKOFF; ++i) {
                                    // Busy-wait
                                }
                            #endif
                        }
                    }
                }
            #elif (VAR == 3)
                void arrive_and_wait(int in_self)
                {
                    // toggle itself
                    static thread_local bool lsense = true;
                    lsense = !lsense;
                    if(cnt.fetch_add(1, std::memory_order_relaxed) == (n_thread - 1))
                    {
                        cnt.store(0, std::memory_order_relaxed);
                        sense.store(lsense, std::memory_order_release);
                    }
                    else
                    {
                        while (sense.load(std::memory_order_acquire) != lsense)
                        {
                            /* spin */
                            #if (BACKOFF > 0)
                                for (int i = 0; i < BACKOFF; ++i) {
                                    // Busy-wait
                                }
                            #endif
                        }
                    }
                }
            #elif (VAR == 4)
                void arrive_and_wait(int in_self)
                {
                    // toggle itself
                    static thread_local bool lsense = true;
                    lsense = !lsense;
                    if(cnt.fetch_add(1, std::memory_order_acq_rel) == (n_thread - 1))
                    {
                        cnt.store(0, std::memory_order_relaxed);
                        sense.store(lsense, std::memory_order_relaxed);
                    }
                    else
                    {
                        while (sense.load(std::memory_order_relaxed) != lsense)
                        {
                            /* spin */
                            #if (BACKOFF > 0)
                                for (int i = 0; i < BACKOFF; ++i) {
                                    // Busy-wait
                                }
                            #endif
                        }
                    }
                }
            #elif (VAR == 5)
                void arrive_and_wait(int in_self)
                {
                    // toggle itself
                    static thread_local bool lsense = true;
                    lsense = !lsense;
                    if(cnt.fetch_add(1, std::memory_order_acq_rel) == (n_thread - 1))
                    {
                        cnt.store(0, std::memory_order_relaxed);
                        sense.store(lsense, std::memory_order_release);
                    }
                    else
                    {
                        while (sense.load(std::memory_order_acquire) != lsense)
                        {
                            /* spin */
                            #if (BACKOFF > 0)
                                for (int i = 0; i < BACKOFF; ++i) {
                                    // Busy-wait
                                }
                            #endif
                        }
                    }
                }
            #elif (VAR == 6)
                void arrive_and_wait(int in_self)
                {
                    // toggle itself
                    static thread_local bool lsense = true;
                    lsense = !lsense;
                    if(cnt.fetch_add(1, std::memory_order_acq_rel) == (n_thread - 1))
                    {
                        cnt.store(0, std::memory_order_relaxed);
                        sense.store(lsense, std::memory_order_release);
                    }
                    else
                    {
                        while (sense.load(std::memory_order_acquire) != lsense)
                        {
                            /* spin */
                            #if (BACKOFF > 0)
                                for (int i = 0; i < BACKOFF; ++i) {
                                    // Busy-wait
                                }
                            #endif
                        }
                    }
                }
            #else
                #error [ERROR] "arrive_and_wait is not implemented for this VAR"
            #endif
        #endif
};

extern mySenseReversingBarrier hreadBar;