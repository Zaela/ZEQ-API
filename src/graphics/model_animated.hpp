
#ifndef _ZEQ_MODEL_ANIMATED_HPP_
#define _ZEQ_MODEL_ANIMATED_HPP_

#include "define.hpp"
#include "model_prototype.hpp"
#include "vertex_buffer.hpp"
#include "transformable.hpp"
#include "axis_aligned_bounding_box.hpp"
#include "fog.hpp"
#include "skeleton.hpp"

class ModelAnimated : public Transformable
{
private:
    Skeleton    m_skeleton;
    
public:
    ModelAnimated(zeq_model_proto_t* proto);
    virtual ~ModelAnimated();
    
    void draw();
};

#endif//_ZEQ_MODEL_ANIMATED_HPP_
