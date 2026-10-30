#ifndef PERFORMANCE_METRICS_H
#define PERFORMANCE_METRICS_H

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <psapi.h>

#include <chrono>
#include <iostream>

struct PerformanceMetrics
{
    double wall_time_ms = 0.0;

    double cpu_time_ms = 0.0;
    double cpu_usage_percent = 0.0;

    SIZE_T current_ram_bytes = 0;
    SIZE_T peak_ram_bytes = 0;

    DWORD thread_count = 0;
};

inline double fileTimeToMs(const FILETIME& ft)
{
    ULARGE_INTEGER value{};

    value.LowPart = ft.dwLowDateTime;
    value.HighPart = ft.dwHighDateTime;

    return static_cast<double>(value.QuadPart) / 10000.0;
}

inline bool collectMetrics(
    double wall_time_ms,
    const FILETIME& start_kernel,
    const FILETIME& start_user,
    PerformanceMetrics& metrics)
{
    FILETIME end_creation{}, end_exit{};
    FILETIME end_kernel{}, end_user{};

    if (!GetProcessTimes(
            GetCurrentProcess(),
            &end_creation,
            &end_exit,
            &end_kernel,
            &end_user))
    {
        return false;
    }

    double kernel_ms =
        fileTimeToMs(end_kernel) -
        fileTimeToMs(start_kernel);

    double user_ms =
        fileTimeToMs(end_user) -
        fileTimeToMs(start_user);

    PROCESS_MEMORY_COUNTERS_EX memory{};

    memory.cb = sizeof(memory);

    if (!GetProcessMemoryInfo(
            GetCurrentProcess(),
            reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&memory),
            sizeof(memory)))
    {
        return false;
    }

    metrics.wall_time_ms = wall_time_ms;

    metrics.cpu_time_ms = kernel_ms + user_ms;

    if (wall_time_ms > 0.0)
    {
        metrics.cpu_usage_percent =
            metrics.cpu_time_ms / wall_time_ms * 100.0;
    }

    metrics.current_ram_bytes = memory.WorkingSetSize;

    metrics.peak_ram_bytes =
        memory.PeakWorkingSetSize;

    return true;
}

inline bool getCpuTime(
    FILETIME& kernel,
    FILETIME& user)
{
    FILETIME creation{}, exit{};

    return GetProcessTimes(
        GetCurrentProcess(),
        &creation,
        &exit,
        &kernel,
        &user);
}

inline void printMetrics(
    const PerformanceMetrics& metrics)
{
    std::cout << "\n===== Performance Metrics =====\n";

    std::cout << "Wall Time: "
              << metrics.wall_time_ms << " ms\n";

    std::cout << "CPU Time: "
              << metrics.cpu_time_ms << " ms\n";

    std::cout << "CPU Usage: "
              << metrics.cpu_usage_percent << " %\n";

    std::cout << "Current RAM: "
              << metrics.current_ram_bytes / 1024.0
              << " KB\n";

    std::cout << "Peak RAM: "
              << metrics.peak_ram_bytes / 1024.0
              << " KB\n";
}

#endif