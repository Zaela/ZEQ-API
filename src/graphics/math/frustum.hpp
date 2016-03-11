
#ifndef _ZEQ_FRUSTUM_HPP_
#define _ZEQ_FRUSTUM_HPP_

#include "define.hpp"
#include "plane.hpp"
#include "mat4.hpp"
#include "axis_aligned_bounding_box.hpp"
#include "math.hpp"

class Frustum
{
private:
    Plane p[6];

    static const uint8_t FAR_PLANE      = 0;
    static const uint8_t NEAR_PLANE     = 1;
    static const uint8_t LEFT_PLANE     = 2;
    static const uint8_t RIGHT_PLANE    = 3;
    static const uint8_t BOTTOM_PLANE   = 4;
    static const uint8_t TOP_PLANE      = 5;

public:
    void set(const Mat4& mat)
    {
        const float* m = mat.ptr();
        
        p[FAR_PLANE].normal.x       = m[ 3] - m[ 2];
        p[FAR_PLANE].normal.y       = m[ 7] - m[ 6];
        p[FAR_PLANE].normal.z       = m[11] - m[10];
        p[FAR_PLANE].d              = m[15] - m[14];
        
        p[NEAR_PLANE].normal.x      = m[ 2];
        p[NEAR_PLANE].normal.y      = m[ 6];
        p[NEAR_PLANE].normal.z      = m[10];
        p[NEAR_PLANE].d             = m[14];
        
        p[LEFT_PLANE].normal.x      = m[ 3] + m[ 0];
        p[LEFT_PLANE].normal.y      = m[ 7] + m[ 4];
        p[LEFT_PLANE].normal.z      = m[11] + m[ 8];
        p[LEFT_PLANE].d             = m[15] + m[12];
        
        p[RIGHT_PLANE].normal.x     = m[ 3] - m[ 0];
        p[RIGHT_PLANE].normal.y     = m[ 7] - m[ 4];
        p[RIGHT_PLANE].normal.z     = m[11] - m[ 8];
        p[RIGHT_PLANE].d            = m[15] - m[12];
        
        p[BOTTOM_PLANE].normal.x    = m[ 3] + m[ 1];
        p[BOTTOM_PLANE].normal.y    = m[ 7] + m[ 5];
        p[BOTTOM_PLANE].normal.z    = m[11] + m[ 9];
        p[BOTTOM_PLANE].d           = m[15] + m[13];
        
        p[TOP_PLANE].normal.x       = m[ 3] - m[ 1];
        p[TOP_PLANE].normal.y       = m[ 7] - m[ 5];
        p[TOP_PLANE].normal.z       = m[11] - m[ 9];
        p[TOP_PLANE].d              = m[15] - m[13];
        
        // Normalize normals
        for (int i = 0; i < 6; i++)
        {
            float len = Math::sqrtReciprocal(p[i].normal.lengthSquared());
            
            p[i].normal *= len;
            p[i].d      *= len;
        }
    }
    
    bool contains(AABB& box)
    {
        for (int i = 0; i < 6; i++)
        {
            if (!box.isInFrontOfPlane(p[i]))
                return false;
        }
        
        return true;
    }
};

#endif//_ZEQ_FRUSTUM_HPP_
