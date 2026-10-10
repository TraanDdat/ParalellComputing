/*----------MCS Lock----------*/
#ifndef MCSLOCK_H
#define MCSLOCK_H

#include <atomic>
#include <thread>
#include "MUTEXLock.h"

class MCSLock : public MUTEXLock
{
    private:
        struct Node
        {
            std::atomic<Node*> next{nullptr};
            std::atomic<bool> locked{false};
        };
        std::atomic<Node*> tail{nullptr};
        static thread_local Node node;
        
        void lock_impl(Node& node);
        void unlock_impl(Node& node);
    public:
        void lock() override;
        void unlock() override;
};

#endif