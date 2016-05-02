
#include "skeleton.hpp"

Skeleton::Skeleton()
: m_boneCount(0),
  m_boneArray(nullptr)
{
    
}

Skeleton::Skeleton(const Skeleton& copy)
: m_boneCount(copy.m_boneCount),
  m_boneArray(nullptr)
{
    if (m_boneCount == 0)
        return;
    
    m_boneArray = new Bone[m_boneCount];
    memcpy(m_boneArray, copy.m_boneArray, sizeof(Bone) * m_boneCount);
}

Skeleton::~Skeleton()
{
    if (m_boneArray)
        delete[] m_boneArray;
}

void Skeleton::init(ConvSkeleton& skele)
{
    if (!skele.hasBones())
        return;
    
    uint32_t count  = skele.getBoneCount();
    Bone* bones     = new Bone[count];
    Bone* cur       = bones;
    
    ConvSkeleton::Bone* srcArray = skele.getBoneArray();
    
    std::function<void(uint32_t, Bone*)> recurse;
    recurse = [&cur, srcArray, &recurse](uint32_t index, Bone* parent)
    {
        Bone* dst               = cur++;
        ConvSkeleton::Bone& src = srcArray[index];
        
        dst->hasAnimFrames = false;
        
        dst->pos    = src.pos;
        dst->rot    = src.rot;
        dst->scale  = src.scale;
        
        Mat4 posMatrix;
        posMatrix.setTranslation(src.pos);
        Mat4 rotMatrix;
        src.rot.getMatrixTransposed(rotMatrix);
        Mat4 scaleMatrix;
        scaleMatrix.setScale(src.scale);
        
        Mat4 localMatrix = posMatrix * rotMatrix * scaleMatrix;
    
        if (parent)
        {
            dst->parentGlobalAnimMatrix = &parent->globalAnimMatrix;
            dst->globalAnimMatrix       = (*dst->parentGlobalAnimMatrix) * localMatrix;
        }
        else
        {
            dst->globalAnimMatrix       = localMatrix;
            dst->parentGlobalAnimMatrix = nullptr;
        }
        
        dst->globalInverseMatrix = dst->globalAnimMatrix;
        dst->globalInverseMatrix.invert();
    
        dst->animHint           = 0;
        dst->attachPointType    = src.attachPointType;
        
        for (uint32_t i : src.children)
        {
            recurse(i, dst);
        }
    };
    
    recurse(0, nullptr);
    
    m_boneCount = count;
    m_boneArray = bones;
}