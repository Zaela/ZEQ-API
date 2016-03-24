
#include "zeq_struct.hpp"
#include "archive.hpp"
#include "conv_material.hpp"
#include "opengl.hpp"
#include "timer.hpp"
#include "camera.hpp"

zeq_t::zeq_t()
: m_prevTime(Clock::microseconds())
{
    
}

zeq_t::~zeq_t()
{
    // Delete all loaded archives
    m_archivesMutex.lock();
    for (auto& p : m_archivesByFileName)
    {
        if (p.second)
            delete p.second;
    }
    m_archivesMutex.unlock();
    
    // Delete the null ConvMaterial
    delete ConvMaterial::Null();
}

// Common
void zeq_t::setPathToEQ(const zeq_path_t& path)
{
    m_pathToEQ = path;
    m_pathToEQ.ensureTrailingSlash();
}

zeq_path_t& zeq_t::getPathToEQ()
{
    return m_pathToEQ;
}

// Archive
void zeq_t::addArchive(const std::string& path, zeq_archive_t* arch)
{
    std::lock_guard<AtomicMutex> lock(m_archivesMutex);
    
    if (m_archivesByFileName.count(path))
        delete m_archivesByFileName[path];
    
    m_archivesByFileName[path] = arch;
}

zeq_archive_t* zeq_t::getArchive(const std::string& path)
{
    std::lock_guard<AtomicMutex> lock(m_archivesMutex);
    
    if (m_archivesByFileName.count(path))
        return m_archivesByFileName[path];
    
    return nullptr;
}

void zeq_t::removeArchive(const std::string& path)
{
    std::lock_guard<AtomicMutex> lock(m_archivesMutex);
    
    if (m_archivesByFileName.count(path))
        m_archivesByFileName[path] = nullptr;
}

/*
** API functions
*/

zeq_t* zeq_init(void)
{
    try
    {
        return new zeq_t;
    }
    catch (...)
    {
        return nullptr;
    }
}

void zeq_deinit(zeq_t* Z)
{
    if (Z)
        delete Z;
}

void zeq_opengl_context_created(zeq_t* Z)
{
    (void)Z;
    OpenGL::loadExtensions();
}

void zeq_thread_sleep(uint32_t milliseconds)
{
    Clock::sleepMilliseconds(milliseconds);
}

uint64_t zeq_clock_get_microseconds()
{
    return Clock::microseconds();
}

void zeq_set_path_to_eq(zeq_t* Z, const char* path)
{
    Z->setPathToEQ(path ? path : std::string());
}

const char* zeq_get_path_to_eq(zeq_t* Z)
{
    return Z->getPathToEQ().c_str();
}

int zeq_get_int(zeq_type_t type, void* object, int field)
{
    switch (type)
    {
    case ZEQ_TIMER:
        return zeq_timer_t::get_int(object, field);
    default:
        return 0;
    }
}

uint32_t zeq_get_uint(zeq_type_t type, void* object, int field)
{
    switch (type)
    {
    case ZEQ_TIMER:
        return zeq_timer_t::get_uint(object, field);
    default:
        return 0;
    }
}

float zeq_get_float(zeq_type_t type, void* object, int field)
{
    switch (type)
    {
    case ZEQ_CAMERA:
        return zeq_camera_t::get_float(object, field);
    default:
        return 0.0f;
    }
}

zeq_vec3_t zeq_get_vec3(zeq_type_t type, void* object, int field)
{
    switch (type)
    {
    case ZEQ_CAMERA:
        return zeq_camera_t::get_vec3(object, field);
    default:
        break;
    }
    
    zeq_vec3_t v;
    v.x = 0;
    v.y = 0;
    v.z = 0;
    return v;
}

void* zeq_get_ptr(zeq_type_t type, void* object, int field)
{
    switch (type)
    {
    case ZEQ_TIMER:
        return zeq_timer_t::get_ptr(object, field);
    default:
        return nullptr;
    }
}

void zeq_set_int(zeq_type_t type, void* object, int field, int value)
{
    (void)type;
    (void)object;
    (void)field;
    (void)value;
}

void zeq_set_uint(zeq_type_t type, void* object, int field, uint32_t value)
{
    (void)type;
    (void)object;
    (void)field;
    (void)value;
}

void zeq_set_float(zeq_type_t type, void* object, int field, float value)
{
    switch (type)
    {
    case ZEQ_CAMERA:
        zeq_camera_t::set_float(object, field, value);
        break;
    default:
        break;
    }
}

void zeq_set_vec3(zeq_type_t type, void* object, int field, float x, float y, float z)
{
    switch (type)
    {
    case ZEQ_CAMERA:
        zeq_camera_t::set_vec3(object, field, x, y, z);
        break;
    default:
        break;
    }
}

void zeq_set_ptr(zeq_type_t type, void* object, int field, void* value)
{
    (void)type;
    (void)object;
    (void)field;
    (void)value;
}

zeq_delta_t zeq_advance(zeq_t* Z)
{
    Z->timerPool().check();
    
    uint64_t p = Z->getPrevTime();
    uint64_t t = Clock::microseconds();
    Z->setPrevTime(t);
    
    t -= p;
    
    zeq_delta_t d;
    d.seconds       = (double)t * 0.000001;
    d.microseconds  = t;
    
    return d;
}

void zeq_draw_begin()
{
    glFrontFace(GL_CCW);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void zeq_draw_end()
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}

void zeq_set_global_background_color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}
