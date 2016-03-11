
#ifndef _ZEQ_MAT4_HPP_
#define _ZEQ_MAT4_HPP_

#include "define.hpp"
#include "math.hpp"
#include "vec3.hpp"
#include <algorithm>

class Mat4
{
private:
    float m[16];

private:
    Mat4(int i)
    {
        (void)i;
    }

public:
    Mat4()
    {
        identity();
    }
    
    void identity()
    {
        m[ 0] = 1;
        m[ 1] = 0;
        m[ 2] = 0;
        m[ 3] = 0;
        
        m[ 4] = 0;
        m[ 5] = 1;
        m[ 6] = 0;
        m[ 7] = 0;
        
        m[ 8] = 0;
        m[ 9] = 0;
        m[10] = 1;
        m[11] = 0;
        
        m[12] = 0;
        m[13] = 0;
        m[14] = 0;
        m[15] = 1;
    }
    
    float* ptr()
    {
        return m;
    }
    
    const float* ptr() const
    {
        return m;
    }
    
    float& operator[](int i)
    {
        return m[i];
    }
    
    float operator[](int i) const
    {
        return m[i];
    }
    
    Mat4 operator+(const Mat4& o)
    {
        Mat4 ret(1);
        
        ret.m[ 0] = m[ 0] + o.m[ 0];
        ret.m[ 1] = m[ 1] + o.m[ 1];
        ret.m[ 2] = m[ 2] + o.m[ 2];
        ret.m[ 3] = m[ 3] + o.m[ 3];
        
        ret.m[ 4] = m[ 4] + o.m[ 4];
        ret.m[ 5] = m[ 5] + o.m[ 5];
        ret.m[ 6] = m[ 6] + o.m[ 6];
        ret.m[ 7] = m[ 7] + o.m[ 7];
        
        ret.m[ 8] = m[ 8] + o.m[ 8];
        ret.m[ 9] = m[ 9] + o.m[ 9];
        ret.m[10] = m[10] + o.m[10];
        ret.m[11] = m[11] + o.m[11];
        
        ret.m[12] = m[12] + o.m[12];
        ret.m[13] = m[13] + o.m[13];
        ret.m[14] = m[14] + o.m[14];
        ret.m[15] = m[15] + o.m[15];
        
        return ret;
    }
    
    Mat4 operator*(const Mat4& o)
    {
        Mat4 ret(1);
        
        ret.m[ 0] = m[ 0] * o.m[ 0] + m[ 4] * o.m[ 1] + m[ 8] * o.m[ 2] + m[12] * o.m[ 3];
        ret.m[ 1] = m[ 1] * o.m[ 0] + m[ 5] * o.m[ 1] + m[ 9] * o.m[ 2] + m[13] * o.m[ 3];
        ret.m[ 2] = m[ 2] * o.m[ 0] + m[ 6] * o.m[ 1] + m[10] * o.m[ 2] + m[14] * o.m[ 3];
        ret.m[ 3] = m[ 3] * o.m[ 0] + m[ 7] * o.m[ 1] + m[11] * o.m[ 2] + m[15] * o.m[ 3];
        
        ret.m[ 4] = m[ 0] * o.m[ 4] + m[ 4] * o.m[ 5] + m[ 8] * o.m[ 6] + m[12] * o.m[ 7];
        ret.m[ 5] = m[ 1] * o.m[ 4] + m[ 5] * o.m[ 5] + m[ 9] * o.m[ 6] + m[13] * o.m[ 7];
        ret.m[ 6] = m[ 2] * o.m[ 4] + m[ 6] * o.m[ 5] + m[10] * o.m[ 6] + m[14] * o.m[ 7];
        ret.m[ 7] = m[ 3] * o.m[ 4] + m[ 7] * o.m[ 5] + m[11] * o.m[ 6] + m[15] * o.m[ 7];
        
        ret.m[ 8] = m[ 0] * o.m[ 8] + m[ 4] * o.m[ 9] + m[ 8] * o.m[10] + m[12] * o.m[11];
        ret.m[ 9] = m[ 1] * o.m[ 8] + m[ 5] * o.m[ 9] + m[ 9] * o.m[10] + m[13] * o.m[11];
        ret.m[10] = m[ 2] * o.m[ 8] + m[ 6] * o.m[ 9] + m[10] * o.m[10] + m[14] * o.m[11];
        ret.m[11] = m[ 3] * o.m[ 8] + m[ 7] * o.m[ 9] + m[11] * o.m[10] + m[15] * o.m[11];
        
        ret.m[12] = m[ 0] * o.m[12] + m[ 4] * o.m[13] + m[ 8] * o.m[14] + m[12] * o.m[15];
        ret.m[13] = m[ 1] * o.m[12] + m[ 5] * o.m[13] + m[ 9] * o.m[14] + m[13] * o.m[15];
        ret.m[14] = m[ 2] * o.m[12] + m[ 6] * o.m[13] + m[10] * o.m[14] + m[14] * o.m[15];
        ret.m[15] = m[ 3] * o.m[12] + m[ 7] * o.m[13] + m[11] * o.m[14] + m[15] * o.m[15];
        
        return ret;
    }
    
    static Mat4 perspective(float fovRadians, float aspectRatio, float nearZ, float farZ)
    {
        Mat4 ret;
        
        float f     = 1.0f / tan(fovRadians * 0.5f);
        float diffZ = nearZ - farZ;
        
        ret.m[ 0] = f / aspectRatio;
        ret.m[ 5] = f;
        ret.m[10] = (farZ + nearZ) / diffZ;
        ret.m[11] = -1.0f;
        ret.m[14] = (2.0f * farZ * nearZ) / diffZ;
        
        return ret;
    }
    
    static Mat4 lookAt(Vec3& pos, Vec3& targ, Vec3& up)
    {
        Vec3 zaxis = targ - pos;
        zaxis.normalize();
        
        Vec3 xaxis = zaxis.crossProduct(up);
        xaxis.normalize();
        
        Vec3 yaxis = xaxis.crossProduct(zaxis);
        
        Mat4 ret(1);
        
        ret.m[ 0] = xaxis.x;
        ret.m[ 1] = yaxis.x;
        ret.m[ 2] = zaxis.x;
        ret.m[ 3] = 0.0f;
        
        ret.m[ 4] = xaxis.y;
        ret.m[ 5] = yaxis.y;
        ret.m[ 6] = zaxis.y;
        ret.m[ 7] = 0.0f;
        
        ret.m[ 8] = xaxis.z;
        ret.m[ 9] = yaxis.z;
        ret.m[10] = zaxis.z;
        ret.m[11] = 0.0f;
        
        ret.m[12] = -xaxis.dotProduct(pos);
        ret.m[13] = -yaxis.dotProduct(pos);
        ret.m[14] = -zaxis.dotProduct(pos);
        ret.m[15] = 1.0f;
        
        return ret;
    }
    
    void setRotation(float x, float y, float z)
    {
        setRotationRadians(Math::toRadians(x), Math::toRadians(y), Math::toRadians(z));
    }
    
    void setRotationRadians(float x, float y, float z)
    {
        float cr = cos(x);
        float sr = sin(x);
        float cp = cos(y);
        float sp = sin(y);
        float cy = cos(z);
        float sy = sin(z);

        m[ 0] = cp * cy;
        m[ 1] = cp * sy;
        m[ 2] = -sp;

        float srsp = sr * sp;
        float crsp = cr * sp;

        m[ 4] = srsp * cy - cr * sy;
        m[ 5] = srsp * sy + cr * cy;
        m[ 6] = sr * cp;

        m[ 8] = crsp * cy + sr * sy;
        m[ 9] = crsp * sy - sr * cy;
        m[10] = cr * cp;
    }
    
    void setRotationRadians(const Vec3& vec)
    {
        setRotationRadians(vec.x, vec.y, vec.z);
    }
    
    void setTranslation(const Vec3& vec)
    {
        m[12] = vec.x;
        m[13] = vec.y;
        m[14] = vec.z;
    }
    
    void translate(const Vec3& vec)
    {
        m[12] += vec.x;
        m[13] += vec.y;
        m[14] += vec.z;
    }
    
    void setScale(const Vec3& vec)
    {
        m[ 0] = vec.x;
        m[ 5] = vec.y;
        m[10] = vec.z;
        m[15] = 1.0f;
    }
    
    static Mat4 scale(float v)
    {
        Mat4 o(1);
        
        o[ 0] = v;
        o[ 1] = 0;
        o[ 2] = 0;
        o[ 3] = 0;
        
        o[ 4] = 0;
        o[ 5] = v;
        o[ 6] = 0;
        o[ 7] = 0;
        
        o[ 8] = 0;
        o[ 9] = 0;
        o[10] = v;
        o[11] = 0;
        
        o[12] = 0;
        o[13] = 0;
        o[14] = 0;
        o[15] = 1;
        
        return o;
    }
    
    void transformVector(Vec3& vec)
    {
        float x = vec.x * m[ 0] + vec.y * m[ 4] + vec.z * m[ 8] + m[12];
        float y = vec.x * m[ 1] + vec.y * m[ 5] + vec.z * m[ 9] + m[13];
        float z = vec.x * m[ 2] + vec.y * m[ 6] + vec.z * m[10] + m[14];
        
        vec.set(x, y, z);
    }
    
    void transformVector(Vec3& dst, const Vec3& src)
    {
        dst.x = src.x * m[ 0] + src.y * m[ 4] + src.z * m[ 8] + m[12];
        dst.y = src.x * m[ 1] + src.y * m[ 5] + src.z * m[ 9] + m[13];
        dst.z = src.x * m[ 2] + src.y * m[ 6] + src.z * m[10] + m[14];
    }
    
    void rotateVector(Vec3& dst, const Vec3& src)
    {
        dst.x = src.x * m[ 0] + src.y * m[ 4] + src.z * m[ 8];
        dst.y = src.x * m[ 1] + src.y * m[ 5] + src.z * m[ 9];
        dst.z = src.x * m[ 2] + src.y * m[ 6] + src.z * m[10];
    }
    
    Mat4 getTransposed()
    {
        Mat4 o(1);
        
        o[ 0] = m[ 0];
        o[ 1] = m[ 4];
        o[ 2] = m[ 8];
        o[ 3] = m[12];

        o[ 4] = m[ 1];
        o[ 5] = m[ 5];
        o[ 6] = m[ 9];
        o[ 7] = m[13];

        o[ 8] = m[ 2];
        o[ 9] = m[ 6];
        o[10] = m[10];
        o[11] = m[14];

        o[12] = m[ 3];
        o[13] = m[ 7];
        o[14] = m[11];
        o[15] = m[15];
        
        return o;
    }
    
    bool invert()
    {
        Mat4 temp(1);
        
        float d = (m[ 0] * m[ 5] - m[ 1] * m[ 4]) * (m[10] * m[15] - m[11] * m[14]) -
                  (m[ 0] * m[ 6] - m[ 2] * m[ 4]) * (m[ 9] * m[15] - m[11] * m[13]) +
                  (m[ 0] * m[ 7] - m[ 3] * m[ 4]) * (m[ 9] * m[14] - m[10] * m[13]) +
                  (m[ 1] * m[ 6] - m[ 2] * m[ 5]) * (m[ 8] * m[15] - m[11] * m[12]) -
                  (m[ 1] * m[ 7] - m[ 3] * m[ 5]) * (m[ 8] * m[14] - m[10] * m[12]) +
                  (m[ 2] * m[ 7] - m[ 3] * m[ 6]) * (m[ 8] * m[13] - m[ 9] * m[12]);
        
        if (fabsf(d) < 0.000001f)
            return false;
        
        d = Math::reciprocal(d);
        
        temp[ 0] = d * (m[ 5] * (m[10] * m[15] - m[11] * m[14]) + m[ 6] * (m[11] * m[13] - m[ 9] * m[15]) + m[ 7] * (m[ 9] * m[14] - m[10] * m[13]));
        temp[ 1] = d * (m[ 9] * (m[ 2] * m[15] - m[ 3] * m[14]) + m[10] * (m[ 3] * m[13] - m[ 1] * m[15]) + m[11] * (m[ 1] * m[14] - m[ 2] * m[13]));
        temp[ 2] = d * (m[13] * (m[ 2] * m[ 7] - m[ 3] * m[ 6]) + m[14] * (m[ 3] * m[ 5] - m[ 1] * m[ 7]) + m[15] * (m[ 1] * m[ 6] - m[ 2] * m[ 5]));
        temp[ 3] = d * (m[ 1] * (m[ 7] * m[10] - m[ 6] * m[11]) + m[ 2] * (m[ 5] * m[11] - m[ 7] * m[ 9]) + m[ 3] * (m[ 6] * m[ 9] - m[ 5] * m[10]));
        temp[ 4] = d * (m[ 6] * (m[ 8] * m[15] - m[11] * m[12]) + m[ 7] * (m[10] * m[12] - m[ 8] * m[14]) + m[ 4] * (m[11] * m[14] - m[10] * m[15]));
        temp[ 5] = d * (m[10] * (m[ 0] * m[15] - m[ 3] * m[12]) + m[11] * (m[ 2] * m[12] - m[ 0] * m[14]) + m[ 8] * (m[ 3] * m[14] - m[ 2] * m[15]));
        temp[ 6] = d * (m[14] * (m[ 0] * m[ 7] - m[ 3] * m[ 4]) + m[15] * (m[ 2] * m[ 4] - m[ 0] * m[ 6]) + m[12] * (m[ 3] * m[ 6] - m[ 2] * m[ 7]));
        temp[ 7] = d * (m[ 2] * (m[ 7] * m[ 8] - m[ 4] * m[11]) + m[ 3] * (m[ 4] * m[10] - m[ 6] * m[ 8]) + m[ 0] * (m[ 6] * m[11] - m[ 7] * m[10]));
        temp[ 8] = d * (m[ 7] * (m[ 8] * m[13] - m[ 9] * m[12]) + m[ 4] * (m[ 9] * m[15] - m[11] * m[13]) + m[ 5] * (m[11] * m[12] - m[ 8] * m[15]));
        temp[ 9] = d * (m[11] * (m[ 0] * m[13] - m[ 1] * m[12]) + m[ 8] * (m[ 1] * m[15] - m[ 3] * m[13]) + m[ 9] * (m[ 3] * m[12] - m[ 0] * m[15]));
        temp[10] = d * (m[15] * (m[ 0] * m[ 5] - m[ 1] * m[ 4]) + m[12] * (m[ 1] * m[ 7] - m[ 3] * m[ 5]) + m[13] * (m[ 3] * m[ 4] - m[ 0] * m[ 7]));
        temp[11] = d * (m[ 3] * (m[ 5] * m[ 8] - m[ 4] * m[ 9]) + m[ 0] * (m[ 7] * m[ 9] - m[ 5] * m[11]) + m[ 1] * (m[ 4] * m[11] - m[ 7] * m[ 8]));
        temp[12] = d * (m[ 4] * (m[10] * m[13] - m[ 9] * m[14]) + m[ 5] * (m[ 8] * m[14] - m[10] * m[12]) + m[ 6] * (m[ 9] * m[12] - m[ 8] * m[13]));
        temp[13] = d * (m[ 8] * (m[ 2] * m[13] - m[ 1] * m[14]) + m[ 9] * (m[ 0] * m[14] - m[ 2] * m[12]) + m[10] * (m[ 1] * m[12] - m[ 0] * m[13]));
        temp[14] = d * (m[12] * (m[ 2] * m[ 5] - m[ 1] * m[ 6]) + m[13] * (m[ 0] * m[ 6] - m[ 2] * m[ 4]) + m[14] * (m[ 1] * m[ 4] - m[ 0] * m[ 5]));
        temp[15] = d * (m[ 0] * (m[ 5] * m[10] - m[ 6] * m[ 9]) + m[ 1] * (m[ 6] * m[ 8] - m[ 4] * m[10]) + m[ 2] * (m[ 4] * m[ 9] - m[ 5] * m[ 8]));
        
        *this = temp;
        
        return true;
    }
    
    static Mat4 angleX(float angle)
    {
        angle = Math::toRadians(angle);
        float cosX = cos(angle);
        float sinX = sin(angle);

        Mat4 m;

        m.m[ 0] = 1;
        m.m[ 1] = 0;
        m.m[ 2] = 0;
        m.m[ 3] = 0;

        m.m[ 4] = 0;
        m.m[ 5] = cosX;
        m.m[ 6] = sinX;
        m.m[ 7] = 0;

        m.m[ 8] = 0;
        m.m[ 9] = -sinX;
        m.m[10] = cosX;
        m.m[11] = 0;

        m.m[12] = 0;
        m.m[13] = 0;
        m.m[14] = 0;
        m.m[15] = 1;

        return m;
    }

    static Mat4 angleY(float angle)
    {
        angle = Math::toRadians(angle);
        float cosY = cos(angle);
        float sinY = sin(angle);

        Mat4 m;

        m.m[ 0] = cosY;
        m.m[ 1] = 0;
        m.m[ 2] = -sinY;
        m.m[ 3] = 0;

        m.m[ 4] = 0;
        m.m[ 5] = 1;
        m.m[ 6] = 0;
        m.m[ 7] = 0;

        m.m[ 8] = sinY;
        m.m[ 9] = 0;
        m.m[10] = cosY;
        m.m[11] = 0;

        m.m[12] = 0;
        m.m[13] = 0;
        m.m[14] = 0;
        m.m[15] = 1;

        return m;
    }

    static Mat4 angleZ(float angle)
    {
        angle = Math::toRadians(angle);
        float cosZ = cos(angle);
        float sinZ = sin(angle);

        Mat4 m;

        m.m[ 0] = cosZ;
        m.m[ 1] = sinZ;
        m.m[ 2] = 0;
        m.m[ 3] = 0;

        m.m[ 4] = -sinZ;
        m.m[ 5] = cosZ;
        m.m[ 6] = 0;
        m.m[ 7] = 0;

        m.m[ 8] = 0;
        m.m[ 9] = 0;
        m.m[10] = 1;
        m.m[11] = 0;

        m.m[12] = 0;
        m.m[13] = 0;
        m.m[14] = 0;
        m.m[15] = 1;

        return m;
    }

    static Mat4 angleYZ(float yAngle, float zAngle)
    {
        return angleY(yAngle) * angleZ(zAngle);
    }

    static Mat4 angleXYZ(Vec3& v)
    {
        return angleX(v.x) * angleY(v.y) * angleZ(v.z);
    }
    
    Vec3 getRotationNoScale() const
    {
        float y = -asinf(std::max(std::min(m[2], 1.0f), -1.0f));
        float c = cosf(y);
        
        float x, z, rx, ry;
        
        if (fabsf(c) >= 0.000001f)
        {
            float invC = 1.0f / c;
            
            rx = m[10] * invC;
            ry = m[ 6] * invC;
            x  = atan2f(ry, rx);
            rx = m[ 0] * invC;
            ry = m[ 1] * invC;
        }
        else
        {
            x  = 0.0f;
            rx = m[ 5];
            ry = -m[4];
        }
        
        z = atan2f(ry, rx);
        
        return Vec3(x, y, z);
    }
    
    // Model type adjustments
    Mat4 adjustForWLD()
    {
        // Scale by -1 on the X axis;
        // Rotate by -90 degrees on the X axis
        Mat4 mat(1);
        
        mat[ 0] = -1.0f;
        mat[ 1] = 0.0f;
        mat[ 2] = 0.0f;
        mat[ 3] = 0.0f;
        mat[ 4] = 0.0f;
        mat[ 5] = 0.000001f;
        mat[ 6] = -1.0f;
        mat[ 7] = 0.0f;
        mat[ 8] = 0.0f;
        mat[ 9] = 1.0f;
        mat[10] = 0.000001f;
        mat[11] = 0.0f;
        mat[12] = 0.0f;
        mat[13] = 0.0f;
        mat[14] = 0.0f;
        mat[15] = 1.0f;
        
        return (*this) * mat;
    }
    
    Mat4 adjustForEQG()
    {
        // Scale by -1 on the Y axis;
        // Rotate by -90 degrees on the X axis
        Mat4 mat(1);
        
        mat[ 0] = 1.0f;
        mat[ 1] = 0.0f;
        mat[ 2] = 0.0f;
        mat[ 3] = 0.0f;
        mat[ 4] = 0.0f;
        mat[ 5] = -0.000001f;
        mat[ 6] = 1.0f;
        mat[ 7] = 0.0f;
        mat[ 8] = 0.0f;
        mat[ 9] = 1.0f;
        mat[10] = 0.000001f;
        mat[11] = 0.0f;
        mat[12] = 0.0f;
        mat[13] = 0.0f;
        mat[14] = 0.0f;
        mat[15] = 1.0f;

        return (*this) * mat;
    }
};

#endif//_ZEQ_MAT4_HPP_
