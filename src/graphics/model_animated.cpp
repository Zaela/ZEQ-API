
#include "model_animated.hpp"

ModelAnimated::ModelAnimated(zeq_model_proto_t* proto)
: Transformable(proto),
  m_skeleton(proto->skeleton())
{
    
}

ModelAnimated::~ModelAnimated()
{
    
}

void ModelAnimated::draw()
{
    
}
