
#include "model_animated.hpp"

ModelAnimated::ModelAnimated(zeq_model_proto_t* proto)
: Transformable(proto),
  m_isEqg(proto->isEqg()),
  m_skeleton(proto->skeleton()),
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
