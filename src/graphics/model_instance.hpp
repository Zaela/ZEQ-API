
#ifndef _ZEQ_MODEL_INSTANCE_HPP_
#define _ZEQ_MODEL_INSTANCE_HPP_

#include "define.hpp"
#include "model_prototype.hpp"
#include "model_static.hpp"
#include "model_animated.hpp"
#include "transformable.hpp"
#include "camera.hpp"

struct zeq_model_inst_t
{
private:
    bool m_isAnimated;
    bool m_isWld;

    union
    {
        Transformable   m_transformable;
        ModelStatic     m_static;
        ModelAnimated   m_animated;
    };
    
public:
    zeq_model_inst_t(zeq_model_proto_t* proto);
    virtual ~zeq_model_inst_t();

    void setModelMatrix(Mat4& matrix) { m_transformable.setModelMatrix(matrix); }
    
    inline void calculateBoundingBox(AABB& box)
    {
        if (!m_isAnimated)
            m_static.calculateBoundingBox(box);
    }
    
    inline void draw()
    {
        if (!m_isAnimated)
            m_static.draw();
        else
            m_animated.draw();
    }
    
    bool isAnimated() const { return m_isAnimated; }
    ModelAnimated& getAnimated() { return m_animated; }
};

#endif//_ZEQ_MODEL_INSTANCE_HPP_
