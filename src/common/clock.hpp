
#ifndef _ZEQ_CLOCKS_HPP_
#define _ZEQ_CLOCKS_HPP_

#include <cstdint>
#include <thread>
#include <chrono>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <poll.h>
#endif

class Clock
{
public:
    static uint64_t milliseconds();
    static uint64_t microseconds();
    static uint64_t unixSeconds();
    static void     sleepMilliseconds(uint32_t ms);
};

#endif//_ZEQ_TIME_HPP_
