
#ifndef _ZEQ_ZONEVIEWER_HPP_
#define _ZEQ_ZONEVIEWER_HPP_

#include "define_window.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "camera.hpp"

struct zeq_zoneviewer_t
{
private:
    zeq_camera_t* m_camera;

    int8_t m_move;
    int8_t m_turn;

    bool m_mouseLook;
    bool m_mouseLookMoved;

    static const int8_t MOVE_FORWARD    = 1;
    static const int8_t MOVE_BACKWARD   = -1;
    static const int8_t TURN_LEFT       = -1;
    static const int8_t TURN_RIGHT      = 1;

    Vec2 m_mouseRel;
    Vec2 m_mousePos;

    float m_farClip;
    
public:
    zeq_zoneviewer_t(zeq_camera_t* cam);
    ~zeq_zoneviewer_t();

    void handleKeyPress(zeq_key_press_t& key);
    void handleKeyRelease(zeq_key_press_t& key);
    void handleMouseMove(zeq_mouse_move_t& mouse);
    void handleMouseClick(zeq_mouse_click_t& mouse);
    void handleMouseRelease(zeq_mouse_click_t& mouse);

    void update(double delta);
};

#endif//_ZEQ_ZONEVIEWER_HPP_
