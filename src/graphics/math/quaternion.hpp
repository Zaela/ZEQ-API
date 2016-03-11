
#ifndef _ZEQ_QUATERNION_HPP_
#define _ZEQ_QUATERNION_HPP_

#include "define.hpp"
#include "mat4.hpp"
#include "math.hpp"

struct Quaternion
{
    float x, y, z, w;
    
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }
    
    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
    
    void getMatrixTransposed(Mat4& m) const
    {
        m[ 0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
        m[ 1] = 2.0f*x*y - 2.0f*z*w;
        m[ 2] = 2.0f*x*z + 2.0f*y*w;
        m[ 3] = 0.0f;
        
        m[ 4] = 2.0f*x*y + 2.0f*z*w;
        m[ 5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
        m[ 6] = 2.0f*z*y - 2.0f*x*w;
        m[ 7] = 0.0f;
        
        m[ 8] = 2.0f*x*z - 2.0f*y*w;
        m[ 9] = 2.0f*z*y + 2.0f*x*w;
        m[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
        m[11] = 0.0f;
        
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }
    
    Quaternion operator+(const Quaternion& q) const
    {
        return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
    }
    
    Quaternion& operator*=(float v)
    {
        x *= v;
        y *= v;
        z *= v;
        w *= v;
        return *this;
    }
    
    Quaternion operator*(float v)
    {
        return Quaternion(x*v, y*v, z*v, w*v);
    }
    
    float dotProduct(Quaternion& q) const
    {
        return (x * q.x) + (y * q.y) + (z * q.z) + (w * q.w);
    }
    
    Quaternion& slerp(Quaternion a, Quaternion b, float amt)
    {
        float angle = a.dotProduct(b);
        
        if (angle < 0.0f)
        {
            a *= -1.0f;
            angle *= -1.0f;
        }
        
        if (angle <= (1.0f - 0.05f))
        {
            float theta         = acosf(angle);
            float inverseTheta  = Math::reciprocal(sinf(theta));
            float scale         = sinf(theta * (1.0f - amt)) * inverseTheta;
            float inverseScale  = sinf(theta * amt) * inverseTheta;
            return (*this = (a * scale) + (b * inverseScale));
        }
        
        return lerp(a, b, amt);
    }
    
    Quaternion& lerp(Quaternion a, Quaternion b, float amt)
    {
        float scale = 1.0f - amt;
        return (*this = (a * scale) + (b * amt));
    }
};

#endif//_ZEQ_QUATERNION_HPP_
