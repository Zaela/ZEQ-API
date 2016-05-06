
#ifndef _ZEQ_MODEL_PROTOTYPE_HPP_
#define _ZEQ_MODEL_PROTOTYPE_HPP_

#include "define.hpp"
#include "vertex_buffer.hpp"
#include "ref_counter.hpp"
#include "conv_model.hpp"
#include "conv_vertex_buffer.hpp"
#include "skeleton.hpp"
#include "animation.hpp"

struct zeq_model_proto_t : public RefCounter
{
public:
    struct WeightedHead
    {
        uint32_t count;
    };
    
    struct SimpleHead
    {
        uint32_t count;
        union
        {
            VertexBuffer*   vbSingle;
            VertexBuffer**  vbArray;
        };
    };
    
public:
    enum BaseTransform
    {
        NoTransform,
        Wld,
        Eqg
    };
    
private:
    zeq_model_type_t            m_modelType;
    BaseTransform               m_baseTransform;
    std::vector<VertexBuffer*>  m_vertexBuffers;
    std::vector<VertexBuffer*>  m_vertexBuffersNoCollide;
    Skeleton                    m_skeleton;
    AnimationSet                m_animations;
    std::vector<SimpleHead>     m_simpleHeads;
    std::vector<WeightedHead>   m_weightedHeads;

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
    zeq_model_proto_t(ConvModel& model, zeq_model_type_t type, BaseTransform = NoTransform);
    ~zeq_model_proto_t();

    zeq_model_inst_t* createInstance();

    BaseTransform getBaseTransform() const { return m_baseTransform; }
    bool isEqg() const { return m_baseTransform == Eqg; }

    std::vector<VertexBuffer*>& getVertexBuffers() { return m_vertexBuffers; }
    std::vector<VertexBuffer*>& getVertexBuffersNoCollide() { return m_vertexBuffersNoCollide; }
    
    bool isAnimated() const { return m_skeleton.hasBones(); }
    
    void registerWithOpenGLIfNeeded()
    {
        if (!m_registeredWithOpenGL)
            registerWithOpenGL();
    }
    
    Skeleton& skeleton() { return m_skeleton; }
    std::vector<SimpleHead>& getSimpleHeads() { return m_simpleHeads; }
    AnimationSet& animations() { return m_animations; }
};

#endif//_ZEQ_MODEL_PROTOTYPE_HPP_
