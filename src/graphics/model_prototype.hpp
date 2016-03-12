
#ifndef _ZEQ_MODEL_PROTOTYPE_HPP_
#define _ZEQ_MODEL_PROTOTYPE_HPP_

#include "define.hpp"
#include "vertex_buffer.hpp"
#include "ref_counter.hpp"
#include "conv_model.hpp"
#include "conv_vertex_buffer.hpp"

struct zeq_model_proto_t : public RefCounter
{
private:
    zeq_model_type_t            m_modelType;
    std::vector<VertexBuffer*>  m_vertexBuffers;
    std::vector<VertexBuffer*>  m_vertexBuffersNoCollide;

    bool m_registeredWithOpenGL;

    union
    {
        struct
        {
            uint16_t    m_race;
            uint8_t     m_gender;
        };
        uint32_t    m_itemId;
    };
    
private:
    void addVertexBuffers(std::vector<ConvVertexBuffer>& src, std::vector<VertexBuffer*>& dst);
    void registerWithOpenGL();
    
public:
    zeq_model_proto_t(ConvModel& model, zeq_model_type_t type);
    ~zeq_model_proto_t();

    zeq_model_inst_t* createInstance();

    std::vector<VertexBuffer*>& getVertexBuffers() { return m_vertexBuffers; }
    std::vector<VertexBuffer*>& getVertexBuffersNoCollide() { return m_vertexBuffersNoCollide; }
    
    bool isAnimated() { return false; }
};

#endif//_ZEQ_MODEL_PROTOTYPE_HPP_
