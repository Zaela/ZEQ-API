
#include "model_instance.hpp"

zeq_model_inst_t::zeq_model_inst_t(zeq_model_proto_t* proto)
: m_isAnimated(proto->isAnimated()),
  m_isWld(false)
{
    proto->registerWithOpenGLIfNeeded();
    
    if (!m_isAnimated)
    {
        new (&m_static) ModelStatic(proto);
    }
    else
    {
        new (&m_animated) ModelAnimated(proto);
    }
}

zeq_model_inst_t::~zeq_model_inst_t()
{
    if (!m_isAnimated)
    {
        m_static.~ModelStatic();
    }
    else
    {
        m_animated.~ModelAnimated();
    }
}

/*
** API Functions
*/

zeq_model_inst_t* zeq_model_create(zeq_model_proto_t* proto)
{
    try
    {
        return new zeq_model_inst_t(proto);
    }
    catch (...)
    {
        return nullptr;
    }
}

void zeq_model_destroy(zeq_model_inst_t* inst)
{
    if (inst)
        delete inst;
}

void zeq_model_draw(zeq_model_inst_t* inst, zeq_camera_t* cam)
{
    cam->recalculate();
    cam->applyView();
    cam->applyFog();
    
    glMatrixMode(GL_MODELVIEW);
    
    inst->draw();
}

void zeq_model_start_animation(zeq_model_inst_t* inst, int animId)
{
    if (!inst->isAnimated())
        return;
    
    inst->getAnimated().setAnimation(animId);
}

void zeq_model_animate(zeq_model_inst_t* inst, zeq_delta_t delta)
{
    if (!inst->isAnimated())
        return;
    
    AABB box;
    inst->getAnimated().animate(delta.seconds, box);
}
