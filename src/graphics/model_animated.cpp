
#include "model_animated.hpp"

ModelAnimated::ModelAnimated(zeq_model_proto_t* proto)
: Transformable(proto),
  m_isEqg(proto->isEqg()),
  m_skeleton(proto->skeleton()),
  m_animations(proto->animations()),
  m_curAnimId(0),
  m_curAnim(nullptr),
  m_curAnimDuration(0.0f),
  m_curAnimFrame(0.0f),
  m_curHeadSimple(nullptr)
{
    if (m_isEqg)
    {
        new (&m_weightedVBs) std::vector<WeightedVertexBufferSet>;
    }
    else
    {
        new (&m_simpleVBs) std::vector<VertexBufferSet>;
        
        addVertexBuffersSimple(proto->getVertexBuffers());
        addVertexBuffersSimple(proto->getVertexBuffersNoCollide());
        
        new (&m_simpleHeads) std::vector<SimpleHead>;
        
        for (zeq_model_proto_t::SimpleHead& head : proto->getSimpleHeads())
        {
            SimpleHead sh;
            
            sh.count = head.count;
            
            if (head.count == 1)
            {
                sh.single.vertCount = head.vbSingle->count();
                sh.single.src       = head.vbSingle;
                sh.single.dst       = VertexBuffer::copy(*head.vbSingle);
            }
            else
            {
                sh.array = new VertexBufferSet[head.count];
                
                for (uint32_t i = 0; i < head.count; i++)
                {
                    sh.array[i].vertCount   = head.vbArray[i]->count();
                    sh.array[i].src         = head.vbArray[i];
                    sh.array[i].dst         = VertexBuffer::copy(*head.vbArray[i]);
                }
            }
            
            m_simpleHeads.push_back(sh);
        }
        
        if (!m_simpleHeads.empty())
            m_curHeadSimple = &m_simpleHeads[0];
        
        assumeRagdollPosition();
    }
    
    m_animMatrices = new Mat4[m_skeleton.boneCount()];
}

ModelAnimated::~ModelAnimated()
{
    if (m_isEqg)
    {
        
    }
    else
    {
        for (VertexBufferSet& set : m_simpleVBs)
        {
            VertexBuffer::destroy(set.dst);
        }
        
        m_simpleVBs.~vector<VertexBufferSet>();
    }
    
    delete[] m_animMatrices;
}

void ModelAnimated::addVertexBuffersSimple(std::vector<VertexBuffer*>& vbs)
{
    for (VertexBuffer* vb : vbs)
    {
        VertexBufferSet set;
        
        set.vertCount   = vb->count();
        set.src         = vb;
        set.dst         = VertexBuffer::copy(*vb);
        
        m_simpleVBs.push_back(set);
    }
}

void ModelAnimated::draw()
{
    // Assumes matrix mode is GL_MODELVIEW and that the camera view has been applied
    glPushMatrix();
    glMultMatrixf(getModelMatrix().ptr());
    
    zeq_blend_t lastBlend   = ZEQ_BLEND_INVISIBLE;
    uint32_t lastDiffuseId  = 0;
    
    if (m_isEqg)
    {
        
    }
    else
    {
        auto func = [&lastBlend, &lastDiffuseId](VertexBufferSet& set)
        {
            VertexBuffer* vb = set.dst;
            
            if (vb->isInvisible())
                return;
            
            vb->setActiveTexture(lastDiffuseId);
            
            if (vb->setActiveBlend(lastBlend))
            {
                if (lastBlend == ZEQ_BLEND_PARTICLE)
                    Fog::disable();
                else
                    Fog::enable();
            }
            
            vb->draw();
        };
        
        for (VertexBufferSet& set : m_simpleVBs)
        {
            func(set);
        }
        
        if (m_curHeadSimple)
        {
            uint32_t count = m_curHeadSimple->count;
            
            if (count == 1)
            {
                func(m_curHeadSimple->single);
            }
            else
            {
                for (uint32_t i = 0; i < count; i++)
                {
                    func(m_curHeadSimple->array[i]);
                }
            }
        }
    }
    
    zeq_material_t::deactivateBlend(lastBlend);
    
    glPopMatrix();
}

void ModelAnimated::assumeRagdollPosition()
{
    Skeleton::Bone* bones = getModelPrototype()->skeleton().bones();
    
    auto func = [bones](VertexBufferSet& set)
    {
        uint32_t vcount     = set.vertCount;
        VertexBuffer* src   = set.src;
        VertexBuffer* dst   = set.dst;
        
        Vec3 pos;
        Vec3 normal;
        
        for (uint32_t i = 0; i < vcount; i++)
        {
            Vertex& dVert = dst->vertices()[i];
            Vertex& sVert = src->vertices()[i];
            
            Mat4& pull = bones[sVert.boneIndex].globalAnimMatrix;
            
            pull.transformVector(pos, sVert.pos);
            pull.rotateVector(normal, sVert.normal);
            
            dVert.pos    = pos;
            dVert.normal = normal;
        }
    };

    for (VertexBufferSet& set : m_simpleVBs)
    {
        func(set);
    }
    
    if (!m_curHeadSimple)
        return;
    
    uint32_t count = m_curHeadSimple->count;
    
    if (count == 1)
    {
        func(m_curHeadSimple->single);
    }
    else
    {
        for (uint32_t i = 0; i < count; i++)
        {
            func(m_curHeadSimple->array[i]);
        }
    }
}

void ModelAnimated::setAnimation(int animId)
{
    Animation* anim = m_animations.getAnimation(animId);
    
    if (anim)
    {
        m_curAnimId         = animId;
        m_curAnim           = anim;
        m_curAnimDuration   = anim->getDurationMilliseconds();
        m_curAnimFrame      = 0.0f;
        
        Skeleton::Bone* bones = m_skeleton.bones();

        for (uint32_t i = 0; i < m_skeleton.boneCount(); i++)
        {
            bones[i].animHint = Animation::DEFAULT_HINT;
        }
    }
    else
    {
        m_curAnimId         = 0;
        m_curAnim           = nullptr;
        m_curAnimDuration   = 0.0f;
        m_curAnimFrame      = 0.0f;
    }
}

bool ModelAnimated::animate(float delta, AABB& box)
{
    Animation* anim = m_curAnim;
    
    if (!anim)
        return false;
    
    float frame             = incrementAnimation(delta);
    uint32_t count          = m_skeleton.boneCount();
    Skeleton::Bone* bones   = m_skeleton.bones();
    
    // Animate bones
    for (uint32_t i = 0; i < count; i++)
    {
        Skeleton::Bone& bone = bones[i];
        anim->getFrameData(frame, i, bone.pos, bone.rot, bone.scale, bone.animHint);
    }
    
    buildMatrices();
    
    if (m_isEqg)
        box = moveVerticesEQG();
    else
        box = moveVerticesWLD();
    
    return true;
}

float ModelAnimated::incrementAnimation(float delta)
{
    float frame             = m_curAnimFrame + delta;
    uint32_t count          = m_skeleton.boneCount();
    Skeleton::Bone* bone    = m_skeleton.bones();
    
    // Check against anim duration
    while (frame > m_curAnimDuration)
    {
        frame -= m_curAnimDuration;
        for (uint32_t i = 0; i < count; i++)
        {
            bone[i].animHint = Animation::DEFAULT_HINT;
        }
    }
    
    m_curAnimFrame = frame;
    return frame;
}

void ModelAnimated::buildMatrices()
{
    uint32_t count          = m_skeleton.boneCount();
    Skeleton::Bone* bones   = m_skeleton.bones();
    
    for (uint32_t i = 0; i < count; i++)
    {
        Skeleton::Bone& bone = bones[i];
        
        Mat4 mat;
        
        bone.rot.getMatrixTransposed(mat);
        
        Vec3 pos = bone.pos;
        Vec3 scl = bone.scale;
        
        float scale = scl.x;
        mat[ 0] *= scale;
        mat[ 1] *= scale;
        mat[ 2] *= scale;
        mat[ 3] *= scale;
        
        scale = scl.y;
        mat[ 4] *= scale;
        mat[ 5] *= scale;
        mat[ 6] *= scale;
        mat[ 7] *= scale;
        
        scale = scl.z;
        mat[ 8] *= scale;
        mat[ 9] *= scale;
        mat[10] *= scale;
        mat[11] *= scale;
        
        mat[12] = pos.x;
        mat[13] = pos.y;
        mat[14] = pos.z;
        
        if (bone.parentGlobalAnimMatrix)
            bone.globalAnimMatrix = (*bone.parentGlobalAnimMatrix) * mat;
        else
            bone.globalAnimMatrix = mat;
    }
}

AABB ModelAnimated::moveVerticesEQG()
{
    Skeleton::Bone* bones   = m_skeleton.bones();
    uint32_t count          = m_skeleton.boneCount();
    Mat4* animMatrices      = m_animMatrices;
    
    for (uint32_t i = 0; i < count; i++)
    {
        Skeleton::Bone& bone    = bones[i];
        animMatrices[i]         = bone.globalAnimMatrix * bone.globalInverseMatrix;
        
        //if (bone.attachPointSlot != AttachPoint::NONE)
        //    m_attachPoints.setMatrix((AttachPoint::Slot)bone.attachPointSlot, animMatrices[i]);
    }
    
    AABB box;
    
    /*
    for (VertexBufferSet& set : m_vertexBufferSets)
    {
        uint32_t vcount                 = set.vertCount;
        VertexBuffer* src               = set.src;
        VertexBuffer* dst               = set.dst;
        uint32_t n                      = set.assignmentCount;
        WeightedBoneAssignment* bas     = set.assignments;
        
        for (uint32_t i = 0; i < vcount; i++)
        {
            dst[i].moved = false;
        }
        
        Vec3 pos;
        Vec3 normal;
        
        for (uint32_t i = 0; i < n; i++)
        {
            WeightedBoneAssignment& wt  = bas[i];
            uint32_t index              = wt.vertIndex;
            
            Mat4& pull = animMatrices[wt.boneIndex];
            
            pull.transformVector(pos, src[index].pos);
            pull.rotateVector(normal, src[index].normal);
            
            VertexBuffer::Vertex& vert  = dst[index];
            float weight                = wt.weight;
            
            if (!vert.moved)
            {
                vert.moved  = true;
                vert.pos    = pos * weight;
                vert.normal = normal * weight;
            }
            else
            {
                vert.pos    += pos * weight;
                vert.normal += normal * weight;
            }
            
            box.addInternalPoint(vert.pos);
        }
    }
    */
    
    return box;
}

AABB ModelAnimated::moveVerticesWLD()
{
    Skeleton::Bone* bones   = m_skeleton.bones();
    uint32_t count          = m_skeleton.boneCount();
    Mat4* animMatrices      = m_animMatrices;
    
    for (uint32_t i = 0; i < count; i++)
    {
        Skeleton::Bone& bone    = bones[i];
        animMatrices[i]         = bone.globalAnimMatrix;
        
        //if (bone.attachPointSlot != AttachPoint::NONE)
        //    m_attachPoints.setMatrix((AttachPoint::Slot)bone.attachPointSlot, bone.globalAnimMatrix);
    }
    
    AABB box;
    
    auto func = [animMatrices, &box](VertexBufferSet& set)
    {
        uint32_t vcount     = set.vertCount;
        const Vertex* src   = set.src->vertices();
        Vertex* dst         = set.dst->vertices();
        
        Vec3 pos;
        Vec3 normal;
        
        for (uint32_t i = 0; i < vcount; i++)
        {
            Vertex& vert = dst[i];
            
            Mat4& pull = animMatrices[vert.boneIndex];
            
            pull.transformVector(pos, src[i].pos);
            pull.rotateVector(normal, src[i].normal);
            
            vert.pos    = pos;
            vert.normal = normal;
            
            box.addInternalPoint(vert.pos);
        }
    };
    
    for (VertexBufferSet& set : m_simpleVBs)
    {
        func(set);
    }
    
    if (m_curHeadSimple)
    {
        uint32_t count = m_curHeadSimple->count;
        
        if (count == 1)
        {
            func(m_curHeadSimple->single);
        }
        else
        {
            for (uint32_t i = 0; i < count; i++)
            {
                func(m_curHeadSimple->array[i]);
            }
        }
    }
    
    return box;
}
