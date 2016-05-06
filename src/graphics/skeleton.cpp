
#include "skeleton.hpp"

Skeleton::Skeleton()
: m_boneCount(0),
  m_boneArray(nullptr)
{
    
}

Skeleton::Skeleton(const Skeleton& copy)
: m_boneCount(copy.m_boneCount)
{
    Bone* bones     = copy.m_boneArray;
    uint32_t count  = copy.m_boneCount;
    m_boneArray     = new Bone[count];
    memcpy(m_boneArray, copy.m_boneArray, sizeof(Bone) * count);
    
    for (uint32_t i = 0; i < count; i++)
    {
        Bone& bone = bones[i];
        if (bone.parentGlobalAnimMatrix)
        {
            uint32_t c = (uint32_t)(((byte*)bone.parentGlobalAnimMatrix) - ((byte*)bones));
            m_boneArray[i].parentGlobalAnimMatrix = (Mat4*)(((byte*)m_boneArray) + c);
        }
    }
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
    
    ConvSkeleton::Bone* srcArray = skele.getBoneArray();
    
    std::function<void(uint32_t, Bone*)> recurse;
    recurse = [bones, srcArray, &recurse](uint32_t index, Bone* parent)
    {
        Bone* dst               = &bones[index];
        ConvSkeleton::Bone& src = srcArray[index];
        
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
    
        dst->attachPointType = src.attachPointType;
        
        for (uint32_t i : src.children)
        {
            recurse(i, dst);
        }
    };
    
    recurse(0, nullptr);
    
    m_boneCount = count;
    m_boneArray = bones;
}
