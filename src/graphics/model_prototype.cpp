
#include "model_prototype.hpp"
#include "model_instance.hpp"

zeq_model_proto_t::zeq_model_proto_t(ConvModel& model, zeq_model_type_t type, zeq_model_proto_t* inheritAnimsFrom, zeq_model_proto_t::BaseTransform baseTransform)
: m_modelType(type),
  m_baseTransform(baseTransform),
  m_animations(std::move(model.getAnimationSet())),
  m_registeredWithOpenGL(false),
  m_itemId(0)
{
    addVertexBuffers(model.getVertexBuffers(), m_vertexBuffers);
    addVertexBuffers(model.getVertexBuffersNoCollide(), m_vertexBuffersNoCollide);
    
    m_skeleton.init(model.skeleton());
    
    if (inheritAnimsFrom)
    {
        if (m_skeleton.hasMatchingBoneOrder(inheritAnimsFrom->skeleton()))
        {
            m_animations.inherit(inheritAnimsFrom->animations());
        }
        else
        {
            std::unordered_map<uint32_t, uint32_t> reindex;
            m_skeleton.buildBoneReindexingMap(inheritAnimsFrom->skeleton(), reindex);
            m_animations.inheritReindexed(inheritAnimsFrom->animations(), reindex);
        }
    }
    
    if (isEqg())
    {
        
    }
    else
    {
        for (ConvModel* head : model.heads())
        {
            SimpleHead sh;
            
            std::vector<ConvVertexBuffer> vbs;
            
            for (ConvVertexBuffer& vb : head->getVertexBuffers())
            {
                if (!vb.empty())
                    vbs.push_back(std::move(vb));
            }
            
            for (ConvVertexBuffer& vb : head->getVertexBuffersNoCollide())
            {
                if (!vb.empty())
                    vbs.push_back(std::move(vb));
            }
            
            sh.count = vbs.size();
            
            if (sh.count == 0)
                continue;
            
            if (sh.count == 1)
            {
                sh.vbSingle = VertexBuffer::create(vbs[0]);
            }
            else
            {
                sh.vbArray = new VertexBuffer*[sh.count];
                uint32_t i = 0;
                
                for (ConvVertexBuffer& cvb : vbs)
                {
                    sh.vbArray[i++] = VertexBuffer::create(cvb);
                }
            }
            
            m_simpleHeads.push_back(sh);
        }
    }
}

zeq_model_proto_t::~zeq_model_proto_t()
{
    for (VertexBuffer* vb : m_vertexBuffers)
    {
        VertexBuffer::destroy(vb);
    }
}

zeq_model_inst_t* zeq_model_proto_t::createInstance()
{
    if (!m_registeredWithOpenGL)
        registerWithOpenGL();
    
    return new zeq_model_inst_t(this);
}

void zeq_model_proto_t::addVertexBuffers(std::vector<ConvVertexBuffer>& src, std::vector<VertexBuffer*>& dst)
{
    for (ConvVertexBuffer& vb : src)
    {
        if (vb.empty())
            continue;
        
        dst.push_back(VertexBuffer::create(vb));
        vb.clear();
    }
}

void zeq_model_proto_t::registerWithOpenGL()
{
    bool nonAnimated = !isAnimated();
    
    for (VertexBuffer* vb : m_vertexBuffers)
    {
        vb->registerWithOpenGL(nonAnimated, nonAnimated);
    }
    
    for (VertexBuffer* vb : m_vertexBuffersNoCollide)
    {
        vb->registerWithOpenGL(nonAnimated, nonAnimated);
    }
    
    if (isEqg())
    {
        
    }
    else
    {
        for (SimpleHead& head : m_simpleHeads)
        {
            if (head.count == 1)
            {
                head.vbSingle->registerWithOpenGL(nonAnimated, nonAnimated);
            }
            else
            {
                for (uint32_t i = 0; i < head.count; i++)
                {
                    head.vbArray[i]->registerWithOpenGL(nonAnimated, nonAnimated);
                }
            }
        }
    }
    
    m_registeredWithOpenGL = true;
}

/*
** API Functions
*/

void zeq_prototype_destroy(zeq_model_proto_t* proto)
{
    proto->drop();
}
