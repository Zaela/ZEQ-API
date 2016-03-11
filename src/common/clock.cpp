
#include "clock.hpp"
#include "define.hpp"

#ifdef ZEQ_WINDOWS
static LARGE_INTEGER sFrequency;

static LARGE_INTEGER windowsPerfTime()
{
    static BOOL sUseQpc = QueryPerformanceFrequency(&sFrequency);
    
    LARGE_INTEGER now;
    
    HANDLE thread       = GetCurrentThread();
    DWORD_PTR oldMask   = SetThreadAffinityMask(thread, 0);
    
    QueryPerformanceCounter(&now);
    SetThreadAffinityMask(thread, oldMask);
    
    return now;
}
#endif

uint64_t Clock::milliseconds()
{
#ifdef ZEQ_WINDOWS
    LARGE_INTEGER li = windowsPerfTime();
    return (uint64_t)((1000LL * li.QuadPart) / sFrequency.QuadPart);
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000 + t.tv_nsec / 1000000;
#endif
}

uint64_t Clock::microseconds()
{
#ifdef ZEQ_WINDOWS
    LARGE_INTEGER li = windowsPerfTime();
    return (uint64_t)((1000000LL * li.QuadPart) / sFrequency.QuadPart);
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000000 + t.tv_nsec / 1000;
#endif
}

uint64_t Clock::unixSeconds()
{
#ifdef ZEQ_WINDOWS
    return _time64(nullptr);
#else
    return time(nullptr);
#endif
}

void Clock::sleepMilliseconds(uint32_t ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
