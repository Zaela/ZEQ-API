
#ifndef _ZEQ_STRUCT_HPP_
#define _ZEQ_STRUCT_HPP_

#include "define.hpp"
#include "path.hpp"
#include "mutex.hpp"
#include "timer_pool.hpp"
#include "clock.hpp"
#include <vector>
#include <unordered_map>

struct zeq_t
{
private:
    // Common
    zeq_path_t m_pathToEQ;

    uint64_t m_prevTime;

    // Archive
    AtomicMutex m_archivesMutex;
    std::unordered_map<std::string, zeq_archive_t*> m_archivesByFileName;

    TimerPool m_timerPool;

public:
    zeq_t();
    ~zeq_t();

    // Common
    void        setPathToEQ(const zeq_path_t& path);
    zeq_path_t& getPathToEQ();

    uint64_t    getPrevTime() const { return m_prevTime; }
    void        setPrevTime(uint64_t t) { m_prevTime = t; }

    // Archive
    void            addArchive(const std::string& path, zeq_archive_t* arch);
    zeq_archive_t*  getArchive(const std::string& path);
    void            removeArchive(const std::string& path);

    TimerPool&  timerPool() { return m_timerPool; }
};

#endif//_ZEQ_STRUCT_HPP_
