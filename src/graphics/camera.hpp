
#ifndef _ZEQ_CAMERA_HPP_
#define _ZEQ_CAMERA_HPP_

#include "define.hpp"
#include "frustum.hpp"
#include "vec3.hpp"
#include "mat4.hpp"
#include "math.hpp"
#include "opengl.hpp"
#include "ref_counter.hpp"
#include "fog.hpp"

struct zeq_camera_t : public RefCounter
{
private:
    Frustum m_frustum;
    float   m_fieldOfViewRadians;
    float   m_aspectRatio;
    float   m_nearZ;
    float   m_farZ;
    Vec3    m_position;
    Vec3    m_target;
    Vec3    m_up;
    Mat4    m_viewMatrix;
    Mat4    m_projectionMatrix;
    Fog     m_fog;
    
public:
    zeq_camera_t();
    virtual ~zeq_camera_t() { }

    void recalculateView();
    void recalculatePerspective();
    void recalculate();

    void applyView();

    Frustum&    getFrustum() { return m_frustum; }
    Vec3&       getPosition() { return m_position; }
    Vec3&       getTarget() { return m_target; }
    Vec3&       getUpVector() { return m_up; }
    
    void setPosition(Vec3& pos) { m_position = pos; }
    void setTarget(Vec3& target) { m_target = target; }
    void setUpVector(Vec3& up) { m_up = up; }
    
    float getFieldOfViewDegrees() const { return Math::toDegrees(m_fieldOfViewRadians); }
    float getFieldOfViewRadians() const { return m_fieldOfViewRadians; }
    float getAspectRatio() const { return m_aspectRatio; }
    float getNearClip() const { return m_nearZ; }
    float getFarClip() const { return m_farZ; }
    
    void setFieldOfViewDegrees(float deg) { m_fieldOfViewRadians = Math::toRadians(deg); }
    void setFieldOfViewRadians(float rad) { m_fieldOfViewRadians = rad; }
    void setAspectRatio(float r) { m_aspectRatio = r; }
    void setNearClip(float dist) { m_nearZ = dist; }
    void setFarClip(float dist) { m_farZ = dist; }
    
    Fog& getFog() { return m_fog; }
    
    inline void applyFog() { m_fog.apply(); }
    inline void disableFog() { m_fog.disable(); }
    inline void enableFog() { m_fog.enable(); }

    static float get_float(void* object, int field);
    static zeq_vec3_t get_vec3(void* object, int field);
    
    static void set_float(void* object, int field, float value);
    static void set_vec3(void* object, int field, float x, float y, float z);
};

#endif//_ZEQ_CAMERA_HPP_
