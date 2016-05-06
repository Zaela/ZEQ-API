
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
    
    for (uint32_t i = 0; i < count; i++)
    {
        Bone& src = bones[i];
        Bone& dst = m_boneArray[i];
        
        dst = src;
        
        if (src.parentGlobalAnimMatrix)
        {
            uint32_t c = (uint32_t)(((byte*)src.parentGlobalAnimMatrix) - ((byte*)bones));
            dst.parentGlobalAnimMatrix = (Mat4*)(((byte*)m_boneArray) + c);
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
        
        dst->name   = src.name;
        
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

bool Skeleton::hasMatchingBoneOrder(const Skeleton& skele)
{
    uint32_t n = (m_boneCount <= skele.m_boneCount) ? m_boneCount : skele.m_boneCount;
    
    for (uint32_t i = 0; i < n; i++)
    {
        if (m_boneArray[i].name.compare(skele.m_boneArray[i].name) != 0)
            return false;
    }
    
    return true;
}

void Skeleton::buildBoneReindexingMap(const Skeleton& skele, std::unordered_map<uint32_t, uint32_t>& map)
{
    uint32_t dstCount   = m_boneCount;
    uint32_t srcCount   = skele.m_boneCount;
    uint32_t start      = 0;

    for (uint32_t i = 0; i < dstCount; i++)
    {
        Bone& dst = m_boneArray[i];
        
        for (uint32_t j = start; j < srcCount; j++)
        {
            if (dst.name.compare(skele.m_boneArray[j].name) == 0)
            {
                map[j] = i;
                
                if (j == start)
                    start++;

                goto next;
            }
        }
        
    next: ;
    }
}
