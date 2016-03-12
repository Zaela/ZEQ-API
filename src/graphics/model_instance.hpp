
#ifndef _ZEQ_MODEL_INSTANCE_HPP_
#define _ZEQ_MODEL_INSTANCE_HPP_

#include "define.hpp"
#include "model_prototype.hpp"
#include "model_static.hpp"
#include "transformable.hpp"

struct zeq_model_inst_t
{
private:
    bool m_isAnimated;
    bool m_isWld;

    union
    {
        Transformable   m_transformable;
        ModelStatic     m_static;
    };
    
public:
    zeq_model_inst_t(zeq_model_proto_t* proto);
    virtual ~zeq_model_inst_t();

    void setModelMatrix(Mat4& matrix) { m_transformable.setModelMatrix(matrix); }
    
    void calculateBoundingBox(AABB& box);
    void draw();
};

#endif//_ZEQ_MODEL_INSTANCE_HPP_
