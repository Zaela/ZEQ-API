
#ifndef _ZEQ_MUTEX_HPP_
#define _ZEQ_MUTEX_HPP_

#include "define.hpp"
#include <atomic>
#include <mutex>
#include <thread>

#ifndef ZEQ_WINDOWS
#include <pthread.h>
#endif

class AtomicMutex
{
private:
    std::atomic_flag                m_flag;
    std::thread::native_handle_type m_thread;
    int                             m_recurseCount;

private:
    void _lock()
    {
        m_thread = _thread();
        m_recurseCount++;
    }
    
    std::thread::native_handle_type _thread()
    {
    #ifdef ZEQ_WINDOWS
        return ::GetCurrentThread();
    #else
        return ::pthread_self();
    #endif
    }

public:
    AtomicMutex() : m_flag(ATOMIC_FLAG_INIT), m_thread(0), m_recurseCount(0) { }
    
    void lock()
    {
        for (;;)
        {
            if (try_lock())
                return;
        }
    }
    
    void unlock()
    {
        if (m_thread != _thread())
            return;
        
        if (--m_recurseCount > 0)
            return;
        
        m_thread        = 0;
        m_recurseCount  = 0;
        m_flag.clear();
    }
    
    bool try_lock()
    {
        std::thread::native_handle_type t = _thread();
        
        if (m_thread == t || m_flag.test_and_set() == false)
        {
            m_thread = t;
            m_recurseCount++;
            return true;
        }
        
        return false;
    }
};

#endif//_ZEQ_MUTEX_HPP_
