
#include "model_prototype.hpp"
#include "model_instance.hpp"

zeq_model_proto_t::zeq_model_proto_t(ConvModel& model, zeq_model_type_t type, zeq_model_proto_t::BaseTransform baseTransform)
: m_modelType(type),
  m_baseTransform(baseTransform),
  m_registeredWithOpenGL(false),
  m_itemId(0)
{
    addVertexBuffers(model.getVertexBuffers(), m_vertexBuffers);
    addVertexBuffers(model.getVertexBuffersNoCollide(), m_vertexBuffersNoCollide);
    
    m_skeleton.init(model.skeleton());
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
    bool animated = isAnimated();
    
    for (VertexBuffer* vb : m_vertexBuffers)
    {
        vb->registerWithOpenGL(!animated, !animated);
    }
    
    for (VertexBuffer* vb : m_vertexBuffersNoCollide)
    {
        vb->registerWithOpenGL(!animated, !animated);
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
