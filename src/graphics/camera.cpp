
#include "camera.hpp"

zeq_camera_t::zeq_camera_t()
: m_fieldOfViewRadians(Math::toRadians(45)),
  m_aspectRatio(800.0f / 600.0f),
  m_nearZ(0.1f),
  m_farZ(250.0f),
  m_position(0.0f, 0.0f, 0.0f),
  m_target(0.0f, 0.0f, 1.0f),
  m_up(0.0f, 1.0f, 0.0f)
{
    recalculate();
}

void zeq_camera_t::recalculateView()
{
    m_viewMatrix = Mat4::lookAt(m_position, m_target, m_up);
    
    m_frustum.set(m_projectionMatrix * m_viewMatrix);
}

void zeq_camera_t::recalculatePerspective()
{
    m_projectionMatrix = Mat4::perspective(m_fieldOfViewRadians, m_aspectRatio, m_nearZ, m_farZ);
}

void zeq_camera_t::recalculate()
{
    recalculatePerspective();
    recalculateView();
}

void zeq_camera_t::applyView()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_viewMatrix.ptr());
    
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_projectionMatrix.ptr());
}

float zeq_camera_t::get_float(void* object, int field)
{
    zeq_camera_t* cam = (zeq_camera_t*)object;
    
    switch (field)
    {
    case ZEQ_CAMERA_FIELD_OF_VIEW_DEGREES_FLOAT:
        return cam->getFieldOfViewDegrees();
    case ZEQ_CAMERA_FIELD_OF_VIEW_RADIANS_FLOAT:
        return cam->getFieldOfViewRadians();
    case ZEQ_CAMERA_ASPECT_RATIO_FLOAT:
        return cam->getAspectRatio();
    case ZEQ_CAMERA_FAR_CLIP_DISTANCE_FLOAT:
        return cam->getFarClip();
    case ZEQ_CAMERA_NEAR_CLIP_DISTANCE_FLOAT:
        return cam->getNearClip();
    }
    
    return 0.0f;
}

zeq_vec3_t zeq_camera_t::get_vec3(void* object, int field)
{
    zeq_camera_t* cam = (zeq_camera_t*)object;
    
    Vec3 vec;
    
    switch (field)
    {
    case ZEQ_CAMERA_POSITION_VEC3:
        vec = cam->getPosition();
        break;
    case ZEQ_CAMERA_LOOK_AT_VEC3:
        vec = cam->getTarget();
        break;
    case ZEQ_CAMERA_UP_VECTOR_VEC3:
        vec = cam->getUpVector();
        break;
    }
    
    zeq_vec3_t v;
    v.x = vec.x;
    v.y = vec.y;
    v.z = vec.z;
    return v;
}

void zeq_camera_t::set_float(void* object, int field, float value)
{
    zeq_camera_t* cam = (zeq_camera_t*)object;
    
    switch (field)
    {
    case ZEQ_CAMERA_FIELD_OF_VIEW_DEGREES_FLOAT:
        cam->setFieldOfViewDegrees(value);
        break;
    case ZEQ_CAMERA_FIELD_OF_VIEW_RADIANS_FLOAT:
        cam->setFieldOfViewRadians(value);
        break;
    case ZEQ_CAMERA_ASPECT_RATIO_FLOAT:
        cam->setAspectRatio(value);
        break;
    case ZEQ_CAMERA_FAR_CLIP_DISTANCE_FLOAT:
        cam->setFarClip(value);
        break;
    case ZEQ_CAMERA_NEAR_CLIP_DISTANCE_FLOAT:
        cam->setNearClip(value);
        break;
    }
}

void zeq_camera_t::set_vec3(void* object, int field, float x, float y, float z)
{
    zeq_camera_t* cam = (zeq_camera_t*)object;
    
    Vec3 v(x, y, z);
    
    switch (field)
    {
    case ZEQ_CAMERA_POSITION_VEC3:
        cam->setPosition(v);
        break;
    case ZEQ_CAMERA_LOOK_AT_VEC3:
        cam->setTarget(v);
        break;
    case ZEQ_CAMERA_UP_VECTOR_VEC3:
        cam->setUpVector(v);
        break;
    }
}

/*
** API functions
*/

zeq_camera_t* zeq_camera_create()
{
    try
    {
        return new zeq_camera_t;
    }
    catch (...)
    {
        return nullptr;
    }
}

void zeq_camera_destroy(zeq_camera_t* cam)
{
    delete cam;
}
