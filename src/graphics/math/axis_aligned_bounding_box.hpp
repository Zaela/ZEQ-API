
#ifndef _ZEQ_AXIS_ALIGNED_BOUNDING_BOX_HPP_
#define _ZEQ_AXIS_ALIGNED_BOUNDING_BOX_HPP_

#include "define.hpp"
#include "vec3.hpp"
#include "plane.hpp"

// Windows...
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

class AABB
{
private:
    Vec3 m_minCorner;
    Vec3 m_maxCorner;
    
public:
    AABB() : m_minCorner(999999.0f, 999999.0f, 999999.0f), m_maxCorner(-999999.0f, -999999.0f, -999999.0f) { }
    
    AABB(Vec3 min, Vec3 max)
    : m_minCorner(std::min(min.x, max.x), std::min(min.y, max.y), std::min(min.z, max.z)),
      m_maxCorner(std::max(min.x, max.x), std::max(min.y, max.y), std::max(min.z, max.z))
    {
        
    }
    
    Vec3& min() { return m_minCorner; }
    Vec3& max() { return m_maxCorner; }
    
    bool containsPoint(Vec3 v)
    {
        return
            v.x >= m_minCorner.x && v.y >= m_minCorner.y && v.z >= m_minCorner.z &&
            v.x <= m_maxCorner.x && v.y <= m_maxCorner.y && v.z <= m_maxCorner.z;
    }
    
    void addInternalPoint(Vec3 v)
    {
        if (v.x < m_minCorner.x) m_minCorner.x = v.x;
        if (v.y < m_minCorner.y) m_minCorner.y = v.y;
        if (v.z < m_minCorner.z) m_minCorner.z = v.z;
        
        if (v.x > m_maxCorner.x) m_maxCorner.x = v.x;
        if (v.y > m_maxCorner.y) m_maxCorner.y = v.y;
        if (v.z > m_maxCorner.z) m_maxCorner.z = v.z;
    }
    
    Vec3 getCenter()
    {
        Vec3 sphere = m_minCorner + m_maxCorner;
        sphere *= 0.5f;
        return sphere;
    }
    
    float getSphereRadiusSquared()
    {
        Vec3 sphere = getCenter();
        sphere -= m_minCorner;
        return sphere.lengthSquared();
    }
    
    void getAllCorners(Vec3* out, Vec3& center)
    {
        Vec3 mid    = getCenter();
        Vec3 diag   = mid - m_maxCorner;
        
        center = mid;
        
        out[0].set(mid.x + diag.x, mid.y + diag.y, mid.z + diag.z);
        out[1].set(mid.x + diag.x, mid.y + diag.y, mid.z - diag.z);
        out[2].set(mid.x + diag.x, mid.y - diag.y, mid.z + diag.z);
        out[3].set(mid.x + diag.x, mid.y - diag.y, mid.z - diag.z);
        out[4].set(mid.x - diag.x, mid.y + diag.y, mid.z + diag.z);
        out[5].set(mid.x - diag.x, mid.y + diag.y, mid.z - diag.z);
        out[6].set(mid.x - diag.x, mid.y - diag.y, mid.z + diag.z);
        out[7].set(mid.x - diag.x, mid.y - diag.y, mid.z - diag.z);
    }
    
    bool isInFrontOfPlane(Plane& p)
    {
        // These are deliberately "reversed"
        Vec3 near(m_maxCorner);
        Vec3 far(m_minCorner);
        
        if (p.normal.x > 0)
        {
            near.x  = m_minCorner.x;
            far.x   = m_maxCorner.x;
        }
        
        if (p.normal.y > 0)
        {
            near.y  = m_minCorner.y;
            far.y   = m_maxCorner.y;
        }
        
        if (p.normal.z > 0)
        {
            near.z  = m_minCorner.z;
            far.z   = m_maxCorner.z;
        }
        
        return (p.normal.dotProduct(near) + p.d) > 0 || (p.normal.dotProduct(far) + p.d) > 0;
    }
    
    AABB& operator+=(const Vec3& pos)
    {
        m_maxCorner += pos;
        m_minCorner += pos;
        return *this;
    }
};

#endif//_ZEQ_AXIS_ALIGNED_BOUNDING_BOX_HPP_
