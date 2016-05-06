
#ifndef _ZEQ_FRAGMENTS_HPP_
#define _ZEQ_FRAGMENTS_HPP_

#include "define.hpp"
#include "vec3.hpp"
#include "quaternion.hpp"

struct Fragment
{
    uint32_t    _len;
    uint32_t    _type;
    int         _nameRef;
    
    uint32_t length() const { return _len + 8; }
    uint32_t type() const { return _type; }
    int nameRef() const { return _nameRef; }
};

struct SimpleRefFrag : public Fragment
{
    int         ref;
    uint32_t    flag;
};

typedef SimpleRefFrag Frag05;
typedef SimpleRefFrag Frag11;
typedef SimpleRefFrag Frag2d;

struct Frag03 : public Fragment
{
    int         count;
    uint16_t    stringLen;
    char        string[2]; // Variable-length stub
};

struct Frag04 : public Fragment
{
    uint32_t    flag;
    int         count;
    int         ref;
    
    bool isAnimated() const { return count > 1; }
};

struct Frag04Animated : public Fragment
{
    uint32_t    flag;
    int         count;
    uint32_t    milliseconds;
    int         refList[1]; // Variable-length stub
};

struct Frag10Bone
{
    int         nameref;
    uint32_t    flag;
    int         refA;
    int         refB;
    int         size;
    
    Frag10Bone* next() const
    {
        return (Frag10Bone*)(((byte*)this) + sizeof(Frag10Bone) + size * sizeof(int));
    }
    
    int* indexList() const
    {
        return (int*)(((byte*)this) + sizeof(Frag10Bone));
    }
};

struct Frag10 : public Fragment
{
    uint32_t    flag;
    int         count;
    int         ref;
    
private:
    byte* skipFlags() const
    {
        byte* ptr = ((byte*)this) + sizeof(Frag10);
        
        // Skip optional fields if they exist
        if (flag & 1)
            ptr += 12;
        
        if (flag & 2)
            ptr += 4;
        
        return ptr;
    }
    
public:
    int boneCount() const { return count; }
    
    Frag10Bone* boneList() const { return (Frag10Bone*)skipFlags(); }
    
    int* refList(int& outCount) const
    {
        byte* ptr = skipFlags();
        
        // Skip bones (variable size)
        int n = boneCount();
        
        for (int i = 0; i < n; i++)
        {
            ptr += 16;
            int count = *(int*)ptr;
            ptr += (count + 1) * sizeof(int);
        }
        
        outCount = *(int*)ptr;
        return (int*)(ptr + sizeof(int));
    }
};

struct Frag12Entry
{
    int16_t rotW, rotX, rotY, rotZ;
    int16_t shiftX, shiftY, shiftZ, shiftDenom;
    
    void getPosRot(Vec3& pos, Quaternion& rot)
    {
        if (shiftDenom != 0)
        {
            float denom = (float)shiftDenom;
            
            pos.x = (float)shiftX / denom;
            pos.y = (float)shiftY / denom;
            pos.z = (float)shiftZ / denom;
        }
        
        if (rotW != 0)
        {
            rot.x = -((float)rotX) / 16384.0f;
            rot.y = -((float)rotY) / 16384.0f;
            rot.z = -((float)rotZ) / 16384.0f;
            rot.w =  ((float)rotW) / 16384.0f;
        }
    }
    
    bool operator!=(const Frag12Entry& o)
    {
        return rotW != o.rotW || rotX != o.rotX || rotY != o.rotY || rotZ != o.rotZ ||
               shiftX != o.shiftX || shiftY != o.shiftY || shiftZ != o.shiftZ || shiftDenom != o.shiftDenom;
    }
};

struct Frag12 : public Fragment
{
    uint32_t    flag;
    uint32_t    count;
    Frag12Entry entry[1]; // Variable-length stub
};

struct Frag13 : public Fragment
{
    int         ref;
    uint32_t    flag;
    uint32_t    frameRate;
    
    uint32_t getFrameRate()
    {
        if (length() >= 24)
            return frameRate;
        return 0;
    }
};

struct Frag14 : public Fragment
{
    uint32_t    flag;
    int         refA;
    int         size[2];
    int         refB;
    
    bool hasMeshRefs() const { return size[1] > 0; }
    int getMeshRefCount() const { return size[1]; }
    
    int* refList() const
    {
        int* ptr = (int*)(((byte*)this) + sizeof(Frag14));
        
        // Skip optional fields if they exist
        if (flag & 1)
            ptr++;
        
        if (flag & 2)
            ptr++;
        
        int n = size[0];
        for (int i = 0; i < n; i++)
        {
            ptr += (*ptr) * 2 + 1;
        }
        
        return ptr;
    }
    
    int firstRef() const
    {
        return refList()[0];
    }
};

struct Frag15 : public Fragment
{
    int         nameref;
    uint32_t    flag;
    int         refB;
    float       x, y, z;
    float       rotX, rotY, rotZ;
    float       scaleX, scaleY, scaleZ;
    int         refC;
    uint32_t    refCParam;
};

struct Frag30 : public Fragment
{
    uint32_t    flag;
    uint32_t    visibilityFlag;
    uint32_t    unknown[3];
    int         ref;
    int         unknownB[2];
};

struct Frag31 : public Fragment
{
    uint32_t    flag;
    uint32_t    refCount;
    int         refList[1]; // Variable-length stub
};

struct Frag36 : public Fragment
{
    uint32_t    flag;
    int         materialListRef;
    int         animVertRef;
    int         unknownA[2];
    float       x, y, z;
    float       rotX, rotY, rotZ;
    float       maxDist;
    float       minX, minY, minZ;
    float       maxX, maxY, maxZ;
    uint16_t    vertCount;
    uint16_t    uvCount;
    uint16_t    normalCount;
    uint16_t    colorCount;
    uint16_t    polyCount;
    uint16_t    boneAssignCount;
    uint16_t    polyTextureCount;
    uint16_t    vertTextureCount;
    uint16_t    size9;
    uint16_t    scale;
};

#endif//_ZEQ_FRAGMENTS_HPP_
