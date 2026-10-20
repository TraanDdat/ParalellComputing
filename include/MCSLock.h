/*----------MCS Lock----------*/
#ifndef MCSLOCK_H
#define MCSLOCK_H

#include <atomic>
#include <thread>
#include "MUTEXLock.h"
#include <unordered_map>

class MCSLock : public MUTEXLock
{
    private:
        struct MCSNode
        {
            std::atomic<MCSNode*> next{nullptr};
            std::atomic<bool> waiting{false};
        };
        // Shared by all threads using this lock instance.
        std::atomic<MCSNode*> tail{nullptr};
        // Each thread gets a separate node for each MCSLock instance.
        static MCSNode& getThreadNode(const MCSLock* lock)
        {
            thread_local std::unordered_map<const MCSLock*, MCSNode> nodes;
            return nodes.try_emplace(lock).first->second;
        }
    public:
        MCSLock() = default;
        MCSLock(const MCSLock&) = delete;
        MCSLock& operator=(const MCSLock&) = delete;
        void lock() override;
        void unlock() override;
};
#endif