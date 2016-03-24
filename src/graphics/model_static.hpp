
#ifndef _ZEQ_MODEL_STATIC_HPP_
#define _ZEQ_MODEL_STATIC_HPP_

#include "define.hpp"
#include "model_prototype.hpp"
#include "vertex_buffer.hpp"
#include "transformable.hpp"
#include "axis_aligned_bounding_box.hpp"
#include "fog.hpp"

class ModelStatic : public Transformable
{
public:
    ModelStatic(zeq_model_proto_t* proto);
    virtual ~ModelStatic();
    
    void calculateBoundingBox(AABB& box);
    void draw();
};

#endif//_ZEQ_MODEL_STATIC_HPP_
